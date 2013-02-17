/*
 * uc_dipsw.c
 *
 * Lab 3
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "uc_dipsw.h"

/**
 *Calls gpio_port_dd_get_pin_state() to return the state of the pin connected to p_switch.
 */
int uc_dipsw_get_state(int p_switch){
	return gpio_port_dd_get_pin_state(p_switch-1);
}
/**
 *Calls gpio_port_dd_init() to initialize switches 1, 2, 3, and 4.
 */
void uc_dipsw_init(){
	gpio_port_dd_init(0);
	gpio_port_dd_init(1);
	gpio_port_dd_init(2);
	gpio_port_dd_init(3);
}
