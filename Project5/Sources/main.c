/*
 * main.c
 * 
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */


#include "support_common.h" /* include peripheral declarations and more */

#include "gpt.h"
#include "pit.h"


#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

extern uint32 __VECTOR_RAM[];

static void init();

static void init() {
	program_mode = MODE_IDLE;
	asm_set_ipl(0); // Don't mask any levels
	gpt_port_ta_init(); // button init
}

__declspec(noreturn) int main(void)
{
	init();
	
	while(1) {
	}
}
