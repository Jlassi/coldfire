/*
 * uart.c
 *
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "uart.h"

void uart1_init() {
	// Initialize GPIO port UB (UB = UART1) pins 0 - 3 for primary function
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_UTXD1_UTXD1;
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_URXD1_URXD1;
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_URTS1_URTS1;
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_UCTS1_UCTS1;
	
	MCF_GPIO_DDRUB |= MCF_GPIO_DDRUB_DDRUB0;
	MCF_GPIO_DDRUB &= ~(MCF_GPIO_DDRUB_DDRUB1);
	MCF_GPIO_DDRUB |= MCF_GPIO_DDRUB_DDRUB2;
	MCF_GPIO_DDRUB &= ~(MCF_GPIO_DDRUB_DDRUB3);

	// Reset Receiver in the Command Register (010)
	MCF_UART1_UCR |= MCF_UART_UCR_RESET_RX;
	
	// Reset Transmitter in Command Register (011)
	MCF_UART1_UCR |= MCF_UART_UCR_RESET_TX;
	
	// Set receiver and transmitter clock sources to prescaled system clock
	MCF_UART1_UCSR |= MCF_UART_UCSR_RCS_SYS_CLK;
	MCF_UART1_UCSR |= MCF_UART_UCSR_TCS_SYS_CLK;
	
	// Reset mode register pointer
	MCF_UART1_UCR |= MCF_UART_UCR_RESET_MR;
	
	// UMR1: UART Mode register 1
	// Clear receiver RTS, not using hardware flow control
	MCF_UART1_UMR1 &= ~(MCF_UART_UMR_RXRTS);
	
	// Clear recv interrupt select so interrupts will be generated when a character is recv'd
	MCF_UART1_UMR1 &= ~(MCF_UART_UMR_RXIRQ);
	
	// Clear UMR1[ERR]
	MCF_UART1_UMR1 &= ~(MCF_UART_UMR_ERR);
	
	// Set parity mode to 10, parity type to 0 for no parity
	MCF_UART1_UMR1 |= MCF_UART_UMR_PM(2);
	MCF_UART1_UMR1 &= ~(MCF_UART_UMR_PT);
	
	// Set bits per char to 11 for 8bits/char
	MCF_UART1_UMR1 |= MCF_UART_UMR_BC_8;
	
	// UMR2: UART Mode register 2
	// Set channel mode to 00 (normal)
	MCF_UART1_UMR2 |= MCF_UART_UMR_CM_NORMAL;
	
	// Clear transmitter RTS and CTS, not using hardware flow control
	MCF_UART1_UMR2 &= ~(MCF_UART_UMR_TXRTS);
	MCF_UART1_UMR2 &= ~(MCF_UART_UMR_TXCTS);
	
	// Set stop bit length control to 0111 for 1 stop bit
	MCF_UART1_UMR2 |= MCF_UART_UMR_SB_STOP_BITS_1;
	
	// Configure baud divider. divider = f_sys / (32 * baud_rate)
	//uint64_t divider = 80000000 / (32 * 9600); // ~260
	MCF_UART1_UBG1 |= MCF_UART_UBG1_Divider_MSB(0x01);
	MCF_UART1_UBG2 |= MCF_UART_UBG2_Divider_LSB(0x04);
	
	// Unmask RXRDY and TXRDY interrupts
	MCF_UART1_UIMR &= ~(MCF_UART_UIMR_TXRDY);
	MCF_UART1_UIMR &= ~(MCF_UART_UIMR_FFULL_RXRDY);
	
	// Write UART1 ISR address into the exception vector table (at position 64+55)
	__VECTOR_RAM[64+14] = (uint32)uart1_isr;
	
	// Interrupt Controller: UART1 interrupts as level 6 priority 7 (Source 14)
	MCF_INTC0_ICR14 |= MCF_INTC_ICR_IL(0x06);
	MCF_INTC0_ICR14 |= MCF_INTC_ICR_IP(0x07);
	
	// Clear interrupt source 14 (UART1)
	MCF_INTC0_IMRL &= ~(1 << (14));
	
	// Enable recvr by writing 01 to UCR[RC]
	MCF_UART1_UCR |= MCF_UART_UCR_RX_ENABLED;
	
	// Enable xmittr by writing 01 to UCR[TC]
	MCF_UART1_UCR |= MCF_UART_UCR_TX_ENABLED;
}

// Triggered whenever a character is recv'd
__declspec(interrupt) void uart1_isr() {
	// Read recvieved character into RC
	uint8_t rc = MCF_UART1_URB;
	
	printf("Received character: %02x\n", rc);
}
