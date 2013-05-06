/*
 * main.c
 *
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */


#include "support_common.h" /* include peripheral declarations and more */

#include "led.h"
#include "gpt.h"
#include "nunchuk.h"
#include "pacman.h"
#include "uart.h"

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif


/*
 * Interrupt Priorities
 * PIT0 4/7
 * PIT1 2/7
 * UART 5/7
 * GPT 6/6
 */

// Imported globals
extern uint32 __VECTOR_RAM[];

// Globals
uint32_t g_program_mode;

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

// Changes the program mode and all associated state variables across the program
void switch_prog_mode() {
	if(g_program_mode == MODE_PAUSE) { // Change to game mode
		// Disable uart
		uart1_disable();
		
		// Start the game
		pacman_start();
		
		g_program_mode = MODE_PLAY;
	} else { // Change to download / pause mode
		// Stop the game
		pacman_stop();
		
		// Enable uart
		uart1_enable();
		
		g_program_mode = MODE_PAUSE;
	}
}

__declspec(noreturn) int main(void)
{
	// Init
	asm_set_ipl(0); // Don't mask any levels
	uart1_init();
	gpt_port_ta_init(); // button init
	pacman_init();
	nunchuk_init();
	led_init();
	
	g_program_mode = MODE_PAUSE;
	uart1_enable();
	
	while(1) {
	}
}
