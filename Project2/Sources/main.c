/*******************************************************************************
 * FILE: main.c
 *
 * DECRIPTION
 * A first TWR-MCF5225X program. Uses the GPIO module and Port TC to flash the LED's 
 on the TWR-MCF5225x micro-controller board.
 *
 * AUTHORS
 * Ramsey Kant
 * Micahel Steptoe
 *
 * MODIFICATION HISTORY
 *******************************************************************************
 * 30-JAN-2013 Initial revision
 *******************************************************************************/

#include "support_common.h"

/******************************************************************************
 * FUNCTION: busy_delay()
 *
 * DESCRIPTION
 * Implements a very sleaz9y busy delay which just kills time. The main problem with 
 this technique is that it is
 * not entirely clear what the value of p_delay should be to delay for n microseconds 
 or m milliseconds. We will
 * see a better way to do this later on using the DMA timers (DTIM module).
 ***************************/
static void busy_delay(int p_delay) {
	int i, x = 1;
	for (i = 0; i < p_delay; ++i) {
		x = x * 2 + 1;
	}
}

/***************************************************************************
 * FUNCTION: led_off()
 *
 * DESCRIPTION
 * Turns LED 1, 2, 3, or 4 off.
 *
 * INPUTS
 * p_led should be 1, 2, 3, or 4.
 *****************************************************************************/

static void led_off(int p_led) {
	MCF_GPIO_CLRTC &= ~(1 << (p_led - 1));
}

/***************************************************************************
 FUNCTION: led_on()
 *
 * DESCRIPTION
 * Turns LED 1, 2, 3, or 4 on.
 *
 * INPUTS
 * p_led should be 1, 2, 3, or 4.
 *****************************************************************************/

static void led_on(int p_led) {
	MCF_GPIO_SETTC |= 1 << (p_led - 1);
}

/**************************************************************************** 
 FUNCTION: main()
 *
 * DESCRIPTION
 * Flashes LED's 1 and 3 on and off at about a 2 Hz rate.
 *
 *   declspace(noreturn) tells the compiler that even though main() returns an int, 
 don't complain about the
 * fact that we do not have a return statement at the end of the function. The C 
 language specification states
 * very clearly that main() must return an int. But, this program will never end, so 
 we forgo the return state-
 * ment.
 ******************************************************************************/
int main() {
	/*
	 * Program Port TC Pin Assignment Register (PTCPAR) so pins 0, 1, 2, and 3 are 
 configured for the general-purpose I/O (GPIO) function.
	 */
	MCF_GPIO_PTCPAR = MCF_GPIO_PTCPAR_PTCPAR0(MCF_GPIO_PTCPAR_DTIN0_GPIO)
					| MCF_GPIO_PTCPAR_PTCPAR1(MCF_GPIO_PTCPAR_DTIN1_GPIO)
					| MCF_GPIO_PTCPAR_PTCPAR2(MCF_GPIO_PTCPAR_DTIN2_GPIO)
					| MCF_GPIO_PTCPAR_PTCPAR3(MCF_GPIO_PTCPAR_DTIN3_GPIO);
	
	/* Program Port TC Data Direction Register (DDRTC) so pins 0, 1, 2, and 3 are 
	 configured as output pins. */
	MCF_GPIO_DDRTC = MCF_GPIO_DDRTC_DDRTC0 | MCF_GPIO_DDRTC_DDRTC1
			| MCF_GPIO_DDRTC_DDRTC2 | MCF_GPIO_DDRTC_DDRTC3;
	
	/* Turn off all LED's. */
	led_off(1);
	led_off(2);
	led_off(3);
	led_off(4);
	
	/* Forever... */
	int i = 0;
	while (1) {
		// Reset all LEDs to off
		led_off(1);
		led_off(2);
		led_off(3);
		led_off(4);
		busy_delay(500000); // Delay a quarter of a second
		
		// Restart counter after 15
		if(i >= 16)
			i = 0;
		
		//printf("i = %i\n", i);
		
		switch(i) {
		case 0:
			break;
		case 1:
			led_on(1);
			break;
		case 2:
			led_on(2);
			break;
		case 3:
			led_on(1);
			led_on(2);
			break;
		case 4:
			led_on(3);
			break;
		case 5:
			led_on(1);
			led_on(3);
			break;
		case 6:
			led_on(2);
			led_on(3);
			break;
		case 7:
			led_on(1);
			led_on(2);
			led_on(3);
			break;
		case 8:
			led_on(4);
			break;
		case 9:
			led_on(1);
			led_on(4);
			break;
		case 10:
			led_on(2);
			led_on(4);
			break;
		case 11:
			led_on(1);
			led_on(2);
			led_on(4);
			break;
		case 12:
			led_on(3);
			led_on(4);
			break;
		case 13:
			led_on(1);
			led_on(3);
			led_on(4);
			break;
		case 14:
			led_on(2);
			led_on(3);
			led_on(4);
			break;
		case 15:
			led_on(1);
			led_on(2);
			led_on(3);
			led_on(4);
			break;
		default:
			break;
		}
		i++;
		busy_delay(500000); // Delay a quarter of a second
	}
}
