/*
 * dtim.c
 * 
 * Lab 3
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "dtim.h"

/*
 * Configures the appropriate DTIM0 registers to delay for p_usecs microseconds. 
 */

void dtim0_delay(int p_usecs) {
	// Set reference register timeout
	MCF_DTIM0_DTRR = MCF_DTIM_DTRR_REF((unsigned long)(p_usecs*100-1));
	
	// Enables the counter
	MCF_DTIM0_DTMR |= MCF_DTIM_DTMR_RST;
	
	while( MCF_DTIM0_DTER != (0x2)) {
		// Spin until the flag is set
	}
	
	// Stop the counter
	MCF_DTIM0_DTMR = 0;
	
	// Reinitialize timer
	dtim0_init();
}

/*
 * Initializes the DTIM0 timer.
 */
void dtim0_init() {
	// Setup DMA Timer 0.
	MCF_DTIM0_DTMR = MCF_DTIM_DTMR_PS(49) 
		| MCF_DTIM_DTMR_CE(MCF_DTIM_DTMR_CE_NONE) 
		| MCF_DTIM_DTMR_CLK_DIV16
		| MCF_DTIM_DTMR_FRR; 
	MCF_DTIM0_DTER |= MCF_DTIM_DTER_REF | MCF_DTIM_DTER_CAP; // Event Register is setup for reference and capture events
	MCF_DTIM0_DTRR = MCF_DTIM_DTRR_REF(0); // Clear reference register
	MCF_DTIM0_DTCR = MCF_DTIM_DTCR_CAP(0); // Clear capture register. Capture is count at which the reference register was met
	MCF_DTIM0_DTCN = MCF_DTIM_DTCN_CNT(0); // Clear counter register
	
}
