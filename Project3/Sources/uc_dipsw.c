/*
 * uc_dipsw.c
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */
#include "uc_dipsw.h"

int uc_dipsw_get_state(int p_switch){
	return gpio_port_dd_get_pin_state(p_switch-1);
}

void uc_dipsw_init(){
	gpio_port_dd_init(0);
	gpio_port_dd_init(1);
	gpio_port_dd_init(2);
	gpio_port_dd_init(3);
}
