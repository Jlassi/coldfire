/*
 * File:		support_common.h
 * Purpose:		Various project configurations.
 *
 * Notes:
 */

#ifndef _SUPPORT_COMMON_H_
#define _SUPPORT_COMMON_H_

/* Enable UART Support. */
#define ENABLE_UART_SUPPORT  0


#define MEMORY_INIT \
	/* Initialize RAMBAR: locate SRAM and validate it */ \
	move.l	%#__RAMBAR + 0x21,d0; \
	movec	d0,RAMBAR;

#define SUPPORT_ROM_TO_RAM 1

/*
 * Include the derivative header files
 */
#include "MCF52259.h"

/*
 * Include the board specific header files
 */
#include "MCF52259_sysinit.h"

/********************************************************************/

//#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MODE_PAUSE 0
#define MODE_PLAY 1

// Globals
extern uint32_t g_program_mode;

asm __declspec(register_abi) void asm_set_ipl(int);
void switch_prog_mode();

#endif /* _SUPPORT_COMMON_H_ */
