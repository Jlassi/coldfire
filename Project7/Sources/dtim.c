/*
 * dtim.c
 * 
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "dtim.h"

/*
 * Configures the appropriate DTIM0 registers to delay for p_usecs microseconds. 
 */
void dtim0_delay_us(int p_usecs) {
	// Reset timer counter
	MCF_DTIM0_DTCN = 0;
	
	// Set reference register timeout
	MCF_DTIM0_DTRR = MCF_DTIM_DTRR_REF((uint32_t)(p_usecs-1));
	
	// Output reference, clear any pending DMA requests
	MCF_DTIM0_DTER |= MCF_DTIM_DTER_REF;
	
	// Enables the counter
	MCF_DTIM0_DTMR |= MCF_DTIM_DTMR_RST;
	
	while( MCF_DTIM0_DTER != (0x2)) {
		// Spin until the flag is set
	}
}

/*
 * Initializes the DTIM0 timer.
 */
void dtim0_init() {
	// Set reset bit then clear it
	MCF_DTIM0_DTMR |= MCF_DTIM_DTMR_RST;
	MCF_DTIM0_DTMR &= ~(MCF_DTIM_DTMR_RST);
	
	/*MCF_DTIM0_DTMR = MCF_DTIM_DTMR_PS(79)
		| MCF_DTIM_DTMR_CE(MCF_DTIM_DTMR_CE_NONE)  // Capture Edge = Neither rising or falling
		| MCF_DTIM_DTMR_CLK_DIV16 // Divide by 16 (n = 4. 2^4)
		| MCF_DTIM_DTMR_FRR; // Free Run (keep counting even when reference register has been passed)*/
	MCF_DTIM0_DTMR = 0x4F0A;
	MCF_DTIM0_DTER |= MCF_DTIM_DTER_REF | MCF_DTIM_DTER_CAP; // Event Register is setup for reference and capture events
	MCF_DTIM0_DTRR = MCF_DTIM_DTRR_REF(0); // Clear reference register
	MCF_DTIM0_DTCR = MCF_DTIM_DTCR_CAP(0); // Clear capture register. Capture is count at which the reference register was met
	MCF_DTIM0_DTCN = MCF_DTIM_DTCN_CNT(0); // Clear counter register
	MCF_DTIM0_DTXMR = 0x40;
}

/*
 * Configures the appropriate DTIM3 registers to delay for p_usecs microseconds. 
 */
void dtim3_delay_us(int p_usecs) {
	// Reset timer counter
	MCF_DTIM3_DTCN = 0;
	
	// Set reference register timeout
	MCF_DTIM3_DTRR = MCF_DTIM_DTRR_REF((uint32_t)(p_usecs-1));
	
	// Output reference, clear any pending DMA requests
	MCF_DTIM3_DTER |= MCF_DTIM_DTER_REF;
	
	// Enables the counter
	MCF_DTIM3_DTMR |= MCF_DTIM_DTMR_RST;
	
	while( MCF_DTIM3_DTER != (0x2)) {
		// Spin until the flag is set
	}
}

/*
 * Initializes the DTIM3 timer.
 */
void dtim3_init() {
	// Set reset bit then clear it
	MCF_DTIM3_DTMR |= MCF_DTIM_DTMR_RST;
	MCF_DTIM3_DTMR &= ~(MCF_DTIM_DTMR_RST);
	
	/*MCF_DTIM3_DTMR = MCF_DTIM_DTMR_PS(79)
		| MCF_DTIM_DTMR_CE(MCF_DTIM_DTMR_CE_NONE)  // Capture Edge = Neither rising or falling
		| MCF_DTIM_DTMR_CLK_DIV16 // Divide by 16 (n = 4. 2^4)
		| MCF_DTIM_DTMR_FRR; // Free Run (keep counting even when reference register has been passed)*/
	MCF_DTIM3_DTMR = 0x4F0A;
	MCF_DTIM3_DTER |= MCF_DTIM_DTER_REF | MCF_DTIM_DTER_CAP; // Event Register is setup for reference and capture events
	MCF_DTIM3_DTRR = MCF_DTIM_DTRR_REF(0); // Clear reference register
	MCF_DTIM3_DTCR = MCF_DTIM_DTCR_CAP(0); // Clear capture register. Capture is count at which the reference register was met
	MCF_DTIM3_DTCN = MCF_DTIM_DTCN_CNT(0); // Clear counter register
	MCF_DTIM3_DTXMR = 0x40;
}
