#include "support_common.h"

#include "pwm.h"


void pwm_init() {	
	// Enable PWM7
	MCF_PWM_PWME |= MCF_PWM_PWME_PWME7;	
	
	// Period
	MCF_PWM_PWMPER7 = 81;
	/*MCF_PWM_PWMPER7 = 238;
	MCF_PWM_PWMDTY7 = 150;
	MCF_PWM_PWMCNT7 = 0;*/

	// 20% duty cycle (1-4/5) = 20%
	MCF_PWM_PWMDTY7 = 65;
	
	// Left aligned
	//MCF_PWM_PWMCAE &= ~(MCF_PWM_PWMCAE_CAE7);
	
	// PWM Channel 7 to CLK source
	MCF_PWM_PWMCLK |= MCF_PWM_PWMCLK_PCLK7;
	
	// PreScale B to 3
	MCF_PWM_PWMPRCLK = 3;
	
	// Scale B to 125
	//MCF_PWM_PWMSCLB |= MCF_PWM_PWMSCLB_SCALEB(125);
	
	// Polarity to 0 (low to high)
	//MCF_PWM_PWMPOL &= ~(MCF_PWM_PWMPOL_PPOL7);
	
	// Write to counter to reset it
	//MCF_PWM_PWMCNT7 |= MCF_PWM_PWMCNT_COUNT(0);
}

void pwm_init2() {
	MCF_PWM_PWMPER7 = 190;
	MCF_PWM_PWMDTY7 = 150;
	MCF_PWM_PWMCNT7 = 0;
}
