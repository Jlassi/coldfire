/*
 * game.h
 *
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef GAME_H_
#define GAME_H_

#include "support_common.h"

#include "led.h"

extern uint8_t **pattern;

void game_init();
int game_in_bounds(int x, int y);
int game_alive(int x, int y);
int game_num_neighbours(int x, int y);
void game_next();

#endif /* GAME_H_ */
