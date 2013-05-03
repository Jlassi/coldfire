/*
 * pit.c
 * 
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "pit.h"

// Globals
uint32_t g_pit0_counter;

void pit0_init() {
	// Clear the enable bit so we can configure the timer
	MCF_PIT0_PCSR &= ~(MCF_PIT_PCSR_EN);
	
	// Write a prescaler of 1 which generates an interrupt every 3ms seconds
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
	MCF_PIT0_PMR = MCF_PIT_PMR_PM(0);
	
	// Interrupt Controller: PIT0 interrupts as level 4 priority 7 (Source 55)
	MCF_INTC0_ICR55 |= MCF_INTC_ICR_IL(0x04);
	MCF_INTC0_ICR55 |= MCF_INTC_ICR_IP(0x07);
	
	// Unmask interrupts from the interrupt source
	MCF_INTC0_IMRH &= ~(1 << (55 - 32));
	
	// Write PIT0 ISR address into the exception vector table (at position 64+55)
	__VECTOR_RAM[64+55] = (uint32)pit0_isr;
	
	g_pit0_counter = 0;
	
	// Enable timer
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_EN;
}

void pit0_stop() {
	MCF_PIT0_PCSR &= ~(MCF_PIT_PCSR_EN);
}

// Interrupt service routine for timer0
__declspec(interrupt) void pit0_isr() {
	// Clear the interrupt request
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIF;
	
	// Disable interrupts
	MCF_PIT0_PCSR &= ~(MCF_PIT_PCSR_PIE);
	
	
	if(g_program_mode == MODE_PLAY) {
		g_pit0_counter++;
		
		// Only make it possible for the game state to progress in PLAY mode
		if((g_pit0_counter % 50) == 0) {
			g_pit0_counter = 0;
			pacman_next();
		}
	} else if(g_program_mode == MODE_PAUSE) {
		led_display_blank();
	}
	
	led_refresh();
	
	// Enable interrupts
	MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIE;
}

void pit1_init() {
	// Clear the enable bit so we can configure the timer
	MCF_PIT1_PCSR &= ~(MCF_PIT_PCSR_EN);
	
	// Write a prescaler of 10 which generates an interrupt every 200ms seconds
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_PRE(0x07);
	
	// Timer will stop when execution is halted by the debugger
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_DBG;
	
	// Allow overwriting over the PIT counter
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_OVW;
	
	// Enable interrupts from PIT1
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIE;
	
	// Clear interrupt flag by writing a 1
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIF;
	
	// When PCNTR1 reaches 0, it is reloaded
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_RLD;
	
	// Write 0 into PIT Modulus register (which will reset it to 0xFFFF)
	MCF_PIT1_PMR = MCF_PIT_PMR_PM(0);
	
	// Interrupt Controller: PIT1 interrupts as level 2 priority 7 (Source 56)
	MCF_INTC0_ICR56 |= MCF_INTC_ICR_IL(0x02);
	MCF_INTC0_ICR56 |= MCF_INTC_ICR_IP(0x07);
	
	// Unmask interrupts from the interrupt source
	MCF_INTC0_IMRH &= ~(1 << (56 - 32));
	
	// Write PIT1 ISR address into the exception vector table (at position 64+56)
	__VECTOR_RAM[64+56] = (uint32)pit1_isr;
	
	// Enable timer
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_EN;
}

void pit1_stop() {
	MCF_PIT1_PCSR &= ~(MCF_PIT_PCSR_EN);
}

// Interrupt service routine for the timer
__declspec(interrupt) void pit1_isr() {
	// Clear the interrupt request
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIF;
	
	// Disable interrupts
	MCF_PIT1_PCSR &= ~(MCF_PIT_PCSR_PIE);
	
	// Note: The display will flicker because this nunchuk_read calls a method which waits on a blocking DTIM
	// So no matter how fast this timer is, its still bottlenecked by polling DTIM in the other ISR
	nunchuk_read();
	
	// Enable interrupts
	MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIE;
}


