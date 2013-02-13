/*
 * uc_led.c
 *
 *  Created on: Feb 13, 2013
 *      Author: rkant
 */

#include "uc_led.h"

/**
 * Calls uc_led_off on each of the LEDs
 */
void uc_led_all_off() {
	uc_led_off(1);
	uc_led_off(2);
	uc_led_off(3);
	uc_led_off(4);
}

/**
 * Calls uc_led_on on each of the LEDs
 */
void uc_led_all_on() {
	uc_led_on(1);
	uc_led_on(2);
	uc_led_on(3);
	uc_led_on(4);
}

/**
 * Calls uc_led_toggle on each of the LEDs
 */
void uc_led_all_toggle() {
	uc_led_toggle(1);
	uc_led_toggle(2);
	uc_led_toggle(3);
	uc_led_toggle(4);
}

/**
 * Initialze LED facilities
 */
void uc_led_init() {
	// Initialize LED ports for GPIO
	gpio_port_tc_init();
}

/**
 * Sets the corresponding LED to the off state
 */
void uc_led_off(int p_led) {
	gpio_port_tc_set_pin_state(p_led-1, 0);
}

/**
 * Sets the corresponding LED to the on state
 */
void uc_led_on(int p_led) {
	gpio_port_tc_set_pin_state(p_led-1, 1);
}

/**
 * Toggles the corresponding LED
 */
void uc_led_toggle(int p_led) {
	if(gpio_port_tc_get_pin_state(p_led-1) == 1)
		uc_led_off(p_led);
	else
		uc_led_on(p_led);
}
