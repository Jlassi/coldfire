/*
 * led.h
 *
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef LED_H_
#define LED_H_

#include <stdlib.h>

#include "support_common.h"
#include "pit.h"
#include "qspi.h"

#define BLACK 0x00
#define GREEN 0x01
#define RED 0x02
#define ORANGE 0x03

extern uint8_t **pattern;

extern uint8_t led_current_pattern[8][8];
extern uint8_t g_green[8];
extern uint8_t g_red[8];
extern uint8_t g_row;

void led_init();
void led_display_game();
void led_refresh();
void led_write(uint8_t (*pattern)[8][8]);
void led_write_row();


#endif /* LED_H_ */
