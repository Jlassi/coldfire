/*
 * gpt.c
 *
 *  Created on: Feb 25, 2013
 *      Author: michaeltsteptoe
 */

#include "gpt.h"

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
	MCF_GPT_GLTFLG1 |= 0x01;
	
	//Clear the enable bit so we can configure the timer
	MCF_PIT0_PCSR &= ~(MCF_PIT_PCSR_EN);
	
	// Write a prescaler of 7 which generates an interrupt every 0.209 seconds
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_PRE(0x09);
	
	// Enable timer
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_EN;
		

}
