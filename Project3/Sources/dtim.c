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
	
}

/*
 * Initializes the DTIM0 timer.
 */
void ditim0_init(){
	/*
	 * MCF_GPIO_PTAPAR = MCF_GPIO_PTAPAR_PTAPAR0(MCF_GPIO_PTAPAR_ICOC0_GPIO);
	 * MCF_GPIO_DDRTA = MCF_GPIO_DDRTA_DDRTA0;
	 */
	MCF_DTIM0_DTMR = MCF_DTIM_DTMR_PS(16);
	
}
