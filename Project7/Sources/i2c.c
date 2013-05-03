/*
 * i2c.c
 *
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "i2c.h"

/*
 * Initializes I2C0
 */
void i2c_init() {
	// Initialize DMA timer for use in transfer delays
	dtim0_init();
	
	// Configure GPIO port AS for SCL and SDA
	MCF_GPIO_PASPAR |= MCF_GPIO_PASPAR_SCL0_SCL0;
	MCF_GPIO_PASPAR |= MCF_GPIO_PASPAR_SDA0_SDA0;
	
	// Do we need data direction as output?
	/*MCF_GPIO_DDRAS |= MCF_GPIO_DDRAS_DDRAS0;
	MCF_GPIO_DDRAS |= MCF_GPIO_DDRAS_DDRAS1;*/
	
	// Write the I2C board address
	MCF_I2C0_I2ADR |= MCF_I2C_I2ADR_ADR(I2C_ADDR);
	
	// Write the i2c clock rate. f_i2c = 80Mhz / divider
	// divider = 80Mhz / 0.1 Mhz = 800. Closest divider in Table 29.2 896 for 89.29Khz, IC = 0x3A
	MCF_I2C0_I2FDR |= MCF_I2C_I2FDR_IC(0x3A);
	
	// Reset to slave-receiver mode
	i2c_reset();
	
	// If the bus is busy b/c a slave is transmitting, generate a stop bit when switching
	// from master-recviver back to slave-receiver
	if(MCF_I2C0_I2SR & MCF_I2C_I2SR_IBB) {
		MCF_I2C0_I2CR = 0x00; // Board is slave-receiver, i2c disabled, interrupts disabled
		MCF_I2C0_I2CR = 0xA0; // Board is master-receiver, i2c enabled, interrupts, disabled
		uint8_t dummy = MCF_I2C0_I2DR; // Dummy read from slave which is transmitting
		MCF_I2C0_I2SR = 0x00; // Clear arbitration lost flag, clear i2c interrupt request flag
		MCF_I2C0_I2CR = 0x00; // Board is slave-receiver, i2c disabled, interrupts disabled
		MCF_I2C0_I2CR = 0x80; // Enable I2C module
	}
}

/*
 * Spins until the I2C bus becomes idle
 */
void i2c_acquire_bus() {
	// Loop until I2C bus busy (IBB) becomes 0
	while(MCF_I2C0_I2SR & MCF_I2C_I2SR_IBB) {
	}
}

/*
 * Resets the board to slave-receiver mode, disables interrupts (use polling instead, ACK recv'd bytes, and enables I2C module
 */
void i2c_reset() {
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_IEN; // Enable i2c module
	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_IIEN); // Disable interrupts, we use polling instead
	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_MSTA); // Make board a slave
	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_MTX); // Make board a receiver
	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_TXAK); // Automatically ACK recv'd bytes
	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_RSTA); // Don't generate repeated start bits
}

/*
 * Reads count bytes of data from the slave (addr)
 * @param addr Address of the slave to read from
 * @param size Number of bytes to read from the slave
 * @param data Array of bytes to transmit
 * @param delay_us Busy waits for delay_us microseconds following each transferred byte
 */
void i2c_rx(uint8_t addr, int size, uint8_t *data, int delay_us) {
	i2c_acquire_bus(); // Wait for bus to become idle
	i2c_tx_addr(addr, I2C_READ, delay_us); // Send start bit, slave address, and read bit

	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_MTX); // Become a receiver
	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_TXAK); // Configure to ACK each recv'd data byte
	
	// Read a dummy byte in order to complete the mode switch
	uint8_t dummy = i2c_rx_byte(delay_us);
	
	// Master-receivers must generate clock pulses on SCL to recv 8 data bytes from slave
	// Read and ACK up until the last byte (size-2)
	for(int i = 0; i <= (size-2); i++)
		data[i] = i2c_rx_byte(delay_us);
	
	// NACK to stop transmission and read the last byte
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_TXAK;
	data[size-1] = i2c_rx_byte(delay_us);
	
	// Terminate communication
	i2c_rxtx_end();
}

/*
 * Receives a data byte from the slave-transmitter and returns it
 * @param delay_us Delay in microseconds following the read
 * @return Byte read from the line
 */
uint8_t i2c_rx_byte(int delay_us) {
	uint8_t ret = MCF_I2C0_I2DR; // Read, which generates SCL for the slave to transmit
	
	// Wait for transfer to finish
	while(!i2c_tx_complete()) {
	}
	MCF_I2C0_I2SR &= ~(MCF_I2C_I2SR_IIF); // Clear interrupt request flag
	
	// Delay for delay_us following the transfer
	dtim0_delay_us(delay_us);
	return ret;
}

/*
 * Ends communication with the slave by transmitting a stop bit and switching to slave-receiver mode
 */
void i2c_rxtx_end() {
	MCF_I2C0_I2CR &= ~(MCF_I2C_I2CR_MSTA); // Make board a slave
	i2c_reset();
}

/*
 * Transmit byte array to slave (addr)
 * 
 * @param addr Address of the slave
 * @param size Number of bytes in data to transmit
 * @param data Array of bytes
 * @param delay_us Microseconds to delay between each send
 */
void i2c_tx(uint8_t addr, int size, uint8_t *data, int delay_us) {
	i2c_acquire_bus(); // Waits for bus to become idle
	i2c_tx_addr(addr, I2C_WRITE, delay_us); // Transmit start bit, slave address, and write bit
	
	// Send each byte in data
	for(int i = 0; i < size; i++)
		i2c_tx_byte(data[i], delay_us);
	
	// Terminate communication with slave
	i2c_rxtx_end();
}

/*
 * Starts a transmission with the slave by putting the board into master-transmitter mode,
 * sending the slave address and read/write bit.
 * The I2C bus should be idle (i2c_acquire_bus()) before calling this function
 * 
 * @param addr Address of slave
 * @param rw 0x01 for read, 0x00 for write
 * @param delay_us Microseconds to delay between each send
 */
void i2c_tx_addr(uint8_t addr, uint8_t rw, int delay_us) {
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_MTX; // Make board a transmitter
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_MSTA; // Make board a master (which sends the start bit)
	
	// Compound first 7 address bits followed by the rw bit, then send
	uint8_t hello = 0;
	hello |= addr << 1;
	hello |= rw << 0;
	i2c_tx_byte(hello, delay_us);
}

/*
 * Send a single byte of data to the slave
 * 
 * @param data Byte to send
 * @param delay_us Microseconds to delay after the send
 */
void i2c_tx_byte(uint8_t data, int delay_us) {
	//asm_set_ipl(7); // Mask all interrupt levels !!! necessary?
	
	// Write data
	MCF_I2C0_I2DR = data;
	
	// Wait for data to finish transmitting
	while(!i2c_tx_complete()) {
	}
	MCF_I2C0_I2SR &= ~(MCF_I2C_I2SR_IIF); // Clear interrupt request flag
	
	//asm_set_ipl(0); // Unmask all interrupt levels !!! necessary?
	
	// Delay for delay_us following the transfer
	dtim0_delay_us(delay_us);
}

/*
 * Checks interrupt request flag. If its set the transmit completed, otherwise, its still in progress
 * 
 * @return True if the transmit has completed, false if otherwise
 */
int i2c_tx_complete() {
	return (MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF);
}

