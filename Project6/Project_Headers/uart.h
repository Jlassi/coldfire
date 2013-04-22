/*
 * uart.h
 *
 *  Created on: Apr 17, 2013
 *      Author: Ramsey
 */

#ifndef UART_H_
#define UART_H_

#include "support_common.h"

extern uint32 __VECTOR_RAM[];

void uart1_init();
__declspec(interrupt) void uart1_isr();

#endif /* UART_H_ */
