/*
 * gpio.h
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */

#ifndef GPIO_H_
#define GPIO_H_

int gpio_port_dd_get_pin_state(int p_pin);
void gpio_port_dd_init(int p_pin);
int gpio_port_tc_get_pin_state(int p_pin);
void gpio_port_tc_init();
void gpio_port_tc_set_pin_state(int p_pin, int p_state);

#endif /* GPIO_H_ */
