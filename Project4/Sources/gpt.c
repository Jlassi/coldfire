/*
 * gpt.c
 *
 *  Created on: Feb 25, 2013
 *      Author: michaeltsteptoe
 */

#include "gpt.h"
#include "pit.h"

void gpt_port_ta_init() {
	// Program Port TA Pin Assignment Register (PTCPAR) so pin 0 is configured for the GPT function.
	MCF_GPIO_PTAPAR = MCF_GPIO_PTAPAR_PTAPAR0(MCF_GPIO_PTAPAR_ICOC0_ICOC0);
	
	//Clear GPTIOS[IOS0] to enable channel 0 for input capture.
	MCF_GPT_GPTIOS &= ~(MCF_GPT_GPTIOS_IOS0);
	
	//Configure GPTCTL2[EDG0] to capture high-to-low signal changes, i.e., button presses.
	MCF_GPT_GPTCTL2 |= MCF_GPT_GPTCTL2_INPUT0_FALLING;
	
	//Clear GPTDDR[DDRT0] so the channel 0 pin is an input pin.
	MCF_GPT_GPTDDR &= ~(MCF_GPT_GPTDDR_DDRT0);
	
	//Set GPTIE[C0I] to enable channel 0 interrupts.
	MCF_GPT_GPTIE |= MCF_GPT_GPTIE_CI0;
	
	//Set GPTSCR1[GPTEN] to enable the GPT.
	MCF_GPT_GPTSCR1 |= MCF_GPT_GPTSCR1_GPTEN;
	
	//Configure the INTC module appropriately to recognize interrupts from GPT channel 0
	__VECTOR_RAM[64+44] = (uint32)change_tempo;
	
	//For interrupt source 44 write the level into ICR44[IL] and the priority to ICR44[IP]
	MCF_INTC0_ICR44 |= MCF_INTC_ICR_IL(0x01) | MCF_INTC_ICR_IP(0x07);
	MCF_INTC0_IMRH &= ~(0x01 << 12);
}

__declspec(interrupt) void change_tempo(){
	//Clear the GPT channel 0 interrupt request flag.
	MCF_GPT_GPTFLG1 |= 0x01;
	
	// Mask interrupt so change_tempo isn't called multiple times for one press
	MCF_INTC0_IMRH &= ~(0x01 << 12);
	
	btn_press_count++;
	
	if((btn_press_count % 2) == 0) {
		printf("Change tempo!\n");
		if(pit_tempo_dir == 1) {
			if(pit_tempo_bpm < 120) {
				pit_tempo_bpm += 10;
			} else {
				pit_tempo_dir = 0;
				pit_tempo_bpm -= 10;
			}
		} else {
			if(pit_tempo_bpm > 60) {
				pit_tempo_bpm -= 10;
			} else {
				pit_tempo_dir = 1;
				pit_tempo_bpm += 10;
			}
		}
	}
	
	// Unmask interrupt
	MCF_INTC0_IMRH &= ~(0x01 << 12);
}

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
