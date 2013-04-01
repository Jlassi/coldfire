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

#define MODE_IDLE 0
#define MODE_DL 1
#define MODE_GAME 2

uint32 program_mode;

asm __declspec(register_abi) void asm_set_ipl(int)
{
    
    link    A6,#-8
    movem.l D6-D7,(SP)
    
    move.l  D0,D6 		/*get argument */
    move.w  SR,D7       /* current sr    */
    
    move.l  D7,D0       /* prepare return value  */
    andi.l  #0x0700,D0  /* mask out IPL  */
    lsr.l   #8,D0       /* IPL   */
    
    andi.l  #0x07,D6        /* least significant three bits  */
    lsl.l   #8,D6       /* move over to make mask    */
    
    andi.l  #0x0000F8FF,D7  /* zero out current IPL  */
    or.l    D6,D7           /* place new IPL in sr   */
    move.w  D7,SR
    
    movem.l (SP),D6-D7
    lea     8(SP),SP
    unlk    A6
    rts
}

#endif /* _SUPPORT_COMMON_H_ */
