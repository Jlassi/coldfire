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

// Imported globals
extern uint32 __VECTOR_RAM[];

void uart1_init();
void uart1_download();
__declspec(interrupt) void uart1_isr();
void uart1_write(uint8_t data);

#endif /* UART_H_ */
