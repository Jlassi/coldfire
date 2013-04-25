#include "nunchuk.h"

// Globals
void (*g_callback_input)(uint8_t);

/*
 * Initializes nunchuk facilities and starts reading input from the controller
 */
void nunchuk_init() {
	g_callback_input = NULL;
	dtim3_init();
	i2c_init();
	pit1_init(); // 250ms period, calls nunchuk_read() on interrupt
}

/*
 * Set a callback function for when a unique Nunchuk input is read
 * @param Function pointer that accepts a uint8_t representing the Nunchuk input command
 */
void nunchuk_set_input_callback(void (*cback)(uint8_t)) {
	g_callback_input = cback;
}

/*
 * Called every 250ms by PIT1's ISR. Reads input from the controller over I2C and sends to the appropriate handler
 */
void nunchuk_read() {
	// Transmit initialization commands for unencrypted mode
	nunchuk_xmit_cmd(0xF0, 0x55);
	nunchuk_xmit_cmd(0xFB, 0x00);
	nunchuk_xmit_cmd(0x00, 0x00);
	//nunchuk_xmit_cmd(0x40, 0x00); // Authentic nintendo only
	
	// Read controller state (6 bytes)
	uint8_t *state = (uint8_t*)malloc(6);
	i2c_rx(NUNCHUK_I2C_ADDR, 6, state, NUNCHUK_I2C_DELAY_US);
	
	// Send the individual inputs to the callback function
	if(g_callback_input == NULL) {
		printf("nunchuk g_callback_input is null\n");
		return;
	}
	
	// Left / Right joystick movement
	if(state[0] < 30) {
		g_callback_input(NUNCHUK_INPUT_LEFT);
		//printf("Input LEFT %i\n", state[0]);
		
	} else if(state[0] > 200) {
		g_callback_input(NUNCHUK_INPUT_RIGHT);
		//printf("Input RIGHT %i\n", state[0]);
	}
	
	// Up / Down joystick movement
	if(state[1] < 30) {
		g_callback_input(NUNCHUK_INPUT_DOWN);
		//printf("Input DOWN %i\n", state[1]);
	} else if(state[1] > 208) {
		g_callback_input(NUNCHUK_INPUT_UP);
		//printf("Input UP %i\n", state[1]);
	}
	
	// C button
	if((state[5] & (1 << 1)) == 0) {
		//g_callback_input(NUNCHUK_INPUT_C);
		//printf("C button\n");
	}
	
	// Z button
	if((state[5] & (1 << 0)) == 0) {
		//g_callback_input(NUNCHUK_INPUT_C);
		//printf("Z button\n");
	}
	
	free(state);
}

/*
 * Sends a command to a register on the Nunchuk
 * 
 * @param reg Register to send cmd to. If 0x00, only send cmd
 * @param cmd Command byte to send to the nunchuk
 */
void nunchuk_xmit_cmd(uint8_t reg, uint8_t cmd) {
	uint8_t *data;
	unsigned long size = 0;
	if(reg != 0x00) { // Reg and command sent to nunchuck register
		size = 2;
		data = (uint8_t*)malloc(size);
		data[0] = reg;
		data[1] = cmd;
	} else { // Command send to a nunchuk register
		size = 1;
		data = (uint8_t*)malloc(size);
		data[0] = cmd;
	}
	
	i2c_tx(NUNCHUK_I2C_ADDR, size, data, NUNCHUK_I2C_DELAY_US);
	
	dtim3_delay_us(2 * NUNCHUK_I2C_DELAY_US);
	free(data);
}
