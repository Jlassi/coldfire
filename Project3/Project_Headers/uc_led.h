/*
 * uc_led.h
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */

#ifndef UC_LED_H_
#define UC_LED_H_

#include "gpio.h"

void uc_led_all_off();
void uc_led_all_on();
void uc_led_all_toggle();
void uc_led_init();
void uc_led_off(int p_led);
void uc_led_on(int p_led);
void uc_led_toggle(int p_led);

#endif /* UC_LED_H_ */
