/*
 * uc_led.h
 *
 * Lab 3
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
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
