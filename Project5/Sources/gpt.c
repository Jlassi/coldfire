/*
 * gpt.c
 * 
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "gpt.h"

// Globals
uint32_t program_mode;

void gpt_port_ta_init() {
	program_mode = MODE_IDLE;
	
	// Program Port TA Pin Assignment Register (PTCPAR) so pin 0 is configured for the GPT function.
	MCF_GPIO_PTAPAR = MCF_GPIO_PTAPAR_PTAPAR0(MCF_GPIO_PTAPAR_ICOC0_ICOC0);
	
	//Clear GPTIOS[IOS0] to enable channel 0 for input capture.
	MCF_GPT_GPTIOS &= ~(MCF_GPT_GPTIOS_IOS0);
	
	//Configure GPTCTL2[EDG0] to capture low-to-high signal changes, i.e., button presses.
	MCF_GPT_GPTCTL2 |= MCF_GPT_GPTCTL2_INPUT0_RISING;
	
	//Clear GPTDDR[DDRT0] so the channel 0 pin is an input pin.
	MCF_GPT_GPTDDR &= ~(MCF_GPT_GPTDDR_DDRT0);
	
	//Set GPTIE[C0I] to enable channel 0 interrupts.
	MCF_GPT_GPTIE |= MCF_GPT_GPTIE_CI0;
	
	//Set GPTSCR1[GPTEN] to enable the GPT.
	MCF_GPT_GPTSCR1 |= MCF_GPT_GPTSCR1_GPTEN;
	
	//Configure the INTC module appropriately to recognize interrupts from GPT channel 0
	__VECTOR_RAM[64+44] = (uint32)gpt_isr;
	
	//For interrupt source 44 write the level into ICR44[IL] and the priority to ICR44[IP]
	MCF_INTC0_ICR44 |= MCF_INTC_ICR_IL(0x03) | MCF_INTC_ICR_IP(0x07);
	MCF_INTC0_IMRH &= ~(0x01 << 12);
}

__declspec(interrupt) void gpt_isr(){
	//Clear the GPT channel 0 interrupt request flag.
	MCF_GPT_GPTFLG1 |= 0x01;
	
	// Mask interrupt so change_tempo isn't called multiple times for one press
	//MCF_INTC0_IMRH &= ~(0x01 << 12);
	
	// Switch modes
	uint32 old_mode = program_mode;
	switch(program_mode) {
	case MODE_IDLE:
		program_mode = MODE_GAME;
		g_paused = 0;
		break;
	case MODE_GAME:
		program_mode = MODE_IDLE;
		g_paused = 1;
		break;
	default:
		program_mode = MODE_IDLE;
		break;
	}
	
	// Unmask interrupt
	//MCF_INTC0_IMRH &= ~(0x01 << 12);
}

