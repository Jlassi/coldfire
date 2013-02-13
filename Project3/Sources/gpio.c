/*
 * gpio.c
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */

#include "gpio.h"

/**
 * Accesses the SETDD register and returns the state of pin p_pin (0 or 1).
 */
int gpio_port_dd_get_pin_state(int p_pin) {
	return MCF_GPIO_SETDD & (1 << p_pin);
}

/**
 * Configures PDDPAR and DDRDD so pin p_pin of port DD is in GPIO function and the pin data direction is input.
 */
void gpio_port_dd_init(int p_pin) {
	MCF_GPIO_SETDD |= (1 << p_pin);
}

/**
 * Accesses the SETTC register and returns the state of pin p_pin (0 or 1).
 */
int gpio_port_tc_get_pin_state(int p_pin) {
	return MCF_GPIO_SETTC & (1 << p_pin);
}

/**
 * Configures PTCPAR and DDRTC so port TC is in GPIO function and the pin data direction is output.
 */
void gpio_port_tc_init() {
	// Program Port TC Pin Assignment Register (PTCPAR) so pins 0, 1, 2, and 3 are configured for the general-purpose I/O (GPIO) function.
	MCF_GPIO_PTCPAR = MCF_GPIO_PTCPAR_PTCPAR0(MCF_GPIO_PTCPAR_DTIN0_GPIO)
					| MCF_GPIO_PTCPAR_PTCPAR1(MCF_GPIO_PTCPAR_DTIN1_GPIO)
					| MCF_GPIO_PTCPAR_PTCPAR2(MCF_GPIO_PTCPAR_DTIN2_GPIO)
					| MCF_GPIO_PTCPAR_PTCPAR3(MCF_GPIO_PTCPAR_DTIN3_GPIO);
	
	// Program Port TC Data Direction Register (DDRTC) so pins 0, 1, 2, and 3 are configured as output pins
	MCF_GPIO_DDRTC = MCF_GPIO_DDRTC_DDRTC0 | MCF_GPIO_DDRTC_DDRTC1
			| MCF_GPIO_DDRTC_DDRTC2 | MCF_GPIO_DDRTC_DDRTC3;
}

/**
 * Accesses the SETTC register to set the state of pin p_pin to p_state.
 */
void gpio_port_tc_set_pin_state(int p_pin, int p_state) {
	MCF_GPIO_SETTC |= (p_state << p_pin);
}
