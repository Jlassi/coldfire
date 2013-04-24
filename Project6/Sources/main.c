/*
 * main implementation: use this sample to create your own application
 *
 */


#include "support_common.h" /* include peripheral declarations and more */

#include "led.h"
#include "gpt.h"
#include "nunchuk.h"
#include "pacman.h"
#include "dtim.h"

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif


/*
 * Interrupt Priorities
 * PIT0 2/7
 * PIT1 4/7
 * GPT 6/6
 * UART 6/7
 */

extern uint32 __VECTOR_RAM[];

void init();

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

void init() {
	asm_set_ipl(0); // Don't mask any levels
	gpt_port_ta_init(); // button init
	pacman_init();
	nunchuk_init();
	led_init();
}

__declspec(noreturn) int main(void)
{
	init();
	pacman_start();
	
	while(1) {
	}
}
