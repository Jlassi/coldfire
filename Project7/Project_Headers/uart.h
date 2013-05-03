/*
 * uart.h
 *
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef UART_H_
#define UART_H_

#include "support_common.h"

#include "dtim.h"
#include "pacman.h"

// Packet IDs
#define PACKET_ID_DL 0xFF
#define PACKET_ID_GET 0xFD
#define PACKET_ID_ACK 0xFE

// Imported globals
extern uint32 __VECTOR_RAM[];

// Globals
extern uint32_t map_dl_pos_x;
extern uint32_t map_dl_pos_y;
extern uint8_t **dl_map;

void uart1_init();
void uart1_enable();
void uart1_disable();
__declspec(interrupt) void uart1_isr();
void uart1_write(uint8_t data);

#endif /* UART_H_ */
