/*
 * pit.c
 * 
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "pit.h"

void pit_init() {	
	pit_next_play_count = 0;
	pit_counter = 0;
	
	// Clear the enable bit so we can configure the timer
	MCF_PIT0_PCSR &= ~(MCF_PIT_PCSR_EN);
	
	// Write a prescaler of 7 which generates an interrupt every 0.05 seconds
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_PRE(0x05);
	
	// Timer will stop when execution is halted by the debugger
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_DBG;
	
	// Allow overwriting over the PIT counter
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_OVW;
	
	// Enable interrupts from PIT0
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIE;
	
	// Clear interrupt flag by writing a 1
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIF;
	
	// When PCNTR0 reaches 0, it is reloaded
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_RLD;
	
	// Write 0 into PIT Modulus register (which will reset it to 0xFFFF)
	MCF_PIT0_PMR |= MCF_PIT_PMR_PM(0);
	
	// Interrupt Controller: PIT0 interrupts as level 2 priority 7 (Source 55)
	MCF_INTC0_ICR55 |= MCF_INTC_ICR_IL(2);
	MCF_INTC0_ICR55 |= MCF_INTC_ICR_IP(7);
	
	// Unmask interrupts from the interrupt source
	MCF_INTC0_IMRH &= ~(1 << (55 - 32));
	
	// Write PIT0 ISR address into the exception vector table (at position 64+55)
	__VECTOR_RAM[64+55] = (uint32)pit_isr;
	
	// Enable timer
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_EN;
}

// Interrupt service routine for the timer
__declspec(interrupt) void pit_isr() {
	// Clear the interrupt request
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIF;
	
	if(pit_counter == pit_next_play_count) {
	}
	
	pit_counter++;
}


