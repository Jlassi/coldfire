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
	int i =0;
	MCF_DTIM0_DTRR |= MCF_DTIM_DTRR_REF((unsigned long)(p_usecs-1));
	MCF_DTIM0_DTMR |= MCF_DTIM_DTMR_RST;
	while( MCF_DTIM0_DTER != (0x2)){
		printf("DTER = %i,", (int)(MCF_DTIM0_DTER));
		printf("DTCN = %i, DTMR = %i\n", (int)(MCF_DTIM0_DTCN), (int)(MCF_DTIM0_DTMR));
	}
	MCF_DTIM0_DTMR = 0;
	dtim0_init();
}

/*
 * Initializes the DTIM0 timer.
 */
void dtim0_init(){
	/*
	 * MCF_GPIO_PTAPAR = MCF_GPIO_PTAPAR_PTAPAR0(MCF_GPIO_PTAPAR_ICOC0_GPIO);
	 * MCF_GPIO_DDRTA = MCF_GPIO_DDRTA_DDRTA0;
	 */
	MCF_DTIM0_DTMR = MCF_DTIM_DTMR_PS(79) | MCF_DTIM_DTMR_CE(MCF_DTIM_DTMR_CE_NONE) 
			| MCF_DTIM_DTMR_CLK_DIV16 | MCF_DTIM_DTMR_FRR;
	MCF_DTIM0_DTER |= MCF_DTIM_DTER_REF | MCF_DTIM_DTER_CAP;
	MCF_DTIM0_DTRR = MCF_DTIM_DTRR_REF(0);
	MCF_DTIM0_DTCR = MCF_DTIM_DTCR_CAP(0);
	MCF_DTIM0_DTCN = MCF_DTIM_DTCN_CNT(0);
	
}
