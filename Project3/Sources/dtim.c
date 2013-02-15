/*
 * dtim.c
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */

#include "dtim.h"
/*
 *Configures the appropriate DTIM0 registers to delay for p_usecs microseconds. 
 */


void dtim0_delay(int p_usecs){
	MCF_DTIM0_DTRR = (p_usecs-1);
	MCF_DTIM0_DTMR |= 0;
}

/*
 * Initializes the DTIM0 timer.
 */
void ditim0_init(){
	/*
	 * MCF_GPIO_PTAPAR = MCF_GPIO_PTAPAR_PTAPAR0(MCF_GPIO_PTAPAR_ICOC0_GPIO);
	 * MCF_GPIO_DDRTA = MCF_GPIO_DDRTA_DDRTA0;
	 */
	MCF_DTIM0_DTMR = MCF_DTIM_DTMR_PS(79) | MCF_DTIM_DTMR_CE(MCF_DTIM_DTMR_CE_NONE) | MCF_DTIM_DTMR_CLK(MCF_DTIM_DTMR_CLK_DIV16) | MCF_DTIM_DTMR_RST;
	MCF_DTIM0_DTER = 0;
	MCF_DTIM0_DTRR = 0;
	MCF_DTIM0_DTCR = 0;
	MCF_DTIM0_DTCN = 0;
	
}
