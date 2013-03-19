/*
 * gpio.h
 *
 * Lab 3
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "support_common.h"

int gpio_port_dd_get_pin_state(int p_pin);
void gpio_port_dd_init(int p_pin);
int gpio_port_tc_get_pin_state(int p_pin);
void gpio_port_tc_init();
void gpio_port_tc_set_pin_state(int p_pin, int p_state);
void gpio_port_ta_init();

#endif /* GPIO_H_ */
