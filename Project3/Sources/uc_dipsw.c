/*
 * uc_dipsw.c
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */
#include "uc_dipsw.h"

/**
 *Calls gpio_port_dd_get_pin_state() to return the state of the pin connected to p_switch.
 */
int uc_dipsw_get_state(int p_switch){
	return gpio_port_dd_get_pin_state(p_switch-1);
<<<<<<< HEAD
	
=======
>>>>>>> 320df96b8459e70a3bba30b153f8da284be36578
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
