/*
 * dtim.c
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */

/*
 *Configures the appropriate DTIM0 registers to delay for p_usecs microseconds. 
 */
void dtim0_delay(int p_pin){
	
}

/*
 * Initializes the DTIM0 timer.
 */
void ditim0_init(){
	DTMR0 = IPSBAR+0x0
}
