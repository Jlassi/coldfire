/*
 * uart.c
 *
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "uart.h"

#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_GHOST 3

// Globals
uint32_t map_dl_pos_x = 0;
uint32_t map_dl_pos_y = 0;
uint8_t **dl_map;

void uart1_init() {
	// Initialize GPIO port UB (UB = UART1) pins 0 - 3 for primary function
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_UTXD1_UTXD1;
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_URXD1_URXD1;
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_URTS1_URTS1;
	MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_UCTS1_UCTS1;

	// Reset Receiver in the Command Register (010)
	MCF_UART1_UCR = MCF_UART_UCR_RESET_RX;
	
	// Reset Transmitter in Command Register (011)
	MCF_UART1_UCR = MCF_UART_UCR_RESET_TX;
	
	// Reset mode pointer
	MCF_UART1_UCR = MCF_UART_UCR_RESET_MR;
	
	// Unmask RXRDY and TXRDY interrupts by setting TXRDY and RXRDY
	MCF_UART1_UIMR |= MCF_UART_UIMR_TXRDY;
	MCF_UART1_UIMR |= MCF_UART_UIMR_FFULL_RXRDY;
	
	// Write UART1 ISR address into the exception vector table (at position 64+14)
	__VECTOR_RAM[64+14] = (uint32)uart1_isr;
	
	// Interrupt Controller: UART1 interrupts as level 6 priority 7 (Source 14)
	MCF_INTC0_ICR14 |= MCF_INTC_ICR_IL(7);
	MCF_INTC0_ICR14 |= MCF_INTC_ICR_IP(7);
	
	// Clear interrupt source 14 (UART1)
	//MCF_INTC0_IMRL &= ~(1 << (14));
	MCF_INTC0_IMRL &= ~(MCF_INTC_IMRL_MASKALL| MCF_INTC_IMRL_INT_MASK14);
	
	// Initialize input enable control
	MCF_UART1_UACR = 0;
	//MCF_UART1_UACR |= MCF_UART_UACR_IEC;
	
	// Set receiver and transmitter clock sources to prescaled system clock
	MCF_UART1_UCSR = 0 | MCF_UART_UCSR_RCS_SYS_CLK
					   | MCF_UART_UCSR_TCS_SYS_CLK;
	
	// Configure baud divider. divider = f_sys / (32 * baud_rate)
	//uint64_t divider = 80000000 / (32 * 9600); // ~260
	//MCF_UART1_UBG1 = MCF_UART_UBG1_Divider_MSB(0x01);
	//MCF_UART1_UBG2 = MCF_UART_UBG2_Divider_LSB(0x04);
	MCF_UART1_UBG1 = 0x01;
	MCF_UART1_UBG2 = 0x04;
	
	// UMR1: UART Mode register 1
	uint8_t umr1 = 0;
	// Clear receiver RTS, not using hardware flow control
	umr1 &= ~(MCF_UART_UMR_RXRTS);
	
	// Clear recv interrupt select so interrupts will be generated when a character is recv'd
	umr1 &= ~(MCF_UART_UMR_RXIRQ);
	
	// Clear UMR1[ERR]
	umr1 &= ~(MCF_UART_UMR_ERR);
	
	/*// Set parity mode to 10, parity type to 0 for no parity
	//umr1 |= MCF_UART_UMR_PM(2);
	umr1 |= MCF_UART_UMR_PM_NONE;
	umr1 &= ~(MCF_UART_UMR_PT);*/
	
	// Set even parity
	umr1 |= MCF_UART_UMR_PM_EVEN;
	
	// Set bits per char to 11 for 8bits/char
	umr1 |= MCF_UART_UMR_BC_8;
	
	// Set umr1
	MCF_UART1_UMR1 = umr1;

	// UMR2: UART Mode register 2
	// Set channel mode to 00 (normal)
	MCF_UART1_UMR2 |= MCF_UART_UMR_CM_NORMAL;
	
	// Clear transmitter RTS and CTS, not using hardware flow control
	MCF_UART1_UMR2 &= ~(MCF_UART_UMR_TXRTS);
	MCF_UART1_UMR2 &= ~(MCF_UART_UMR_TXCTS);
	
	// Set stop bit length control to 0111 for 1 stop bit
	MCF_UART1_UMR2 |= MCF_UART_UMR_SB_STOP_BITS_1;
	
	// Enable recvr by writing 01 to UCR[RC]
	MCF_UART1_UCR = MCF_UART_UCR_RX_ENABLED;
	
	// Enable xmittr by writing 01 to UCR[TC]
	MCF_UART1_UCR = MCF_UART_UCR_TX_ENABLED;
	
	// Map download variables
	dl_map = (uint8_t**)malloc(8*sizeof(uint8_t*));
	for(int i = 0; i < 8; i++) {
		dl_map[i] = (uint8_t*)malloc(8*sizeof(uint8_t));
	}
	map_dl_pos_x = 0;
	map_dl_pos_y = 0;
}

// Triggered whenever a character is recv'd
__declspec(interrupt) void uart1_isr() {
	if(MCF_UART1_USR & MCF_UART_USR_RXRDY) {
		MCF_UART1_UCR = MCF_UART_UCR_RX_ENABLED;
		uint8_t rc = (uint8_t)MCF_UART1_URB;
		
		if(rc == PACKET_ID_DL) {
			map_dl_pos_x = 0;
			map_dl_pos_y = 0;
			
			// Signal we are ready for the map
			uart1_write(PACKET_ID_GET);
		} else {
			if(map_dl_pos_y > 8 || map_dl_pos_x > 8)
				return;
			
			dl_map[map_dl_pos_y][map_dl_pos_x] = rc;
			
			// If we've written the last byte of the map, send an acknowledge
			if(map_dl_pos_y == 7 && map_dl_pos_x == 7)
				uart1_write(PACKET_ID_ACK);
			
			// Next column
			map_dl_pos_x++;
			
			// Move to the next row
			if(map_dl_pos_x == 8) {
				map_dl_pos_x = 0;
				map_dl_pos_y++;
			}
		}
	}
}

void uart1_write(uint8_t data) {
	MCF_UART1_UTB = data;
}

