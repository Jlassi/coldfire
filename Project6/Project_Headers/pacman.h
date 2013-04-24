/*
 * pacman.h
 *
 * Project 6
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef PACMAN_H_
#define PACMAN_H_

#include "support_common.h"

#include "pit.h"
#include "led.h"

// Map tile types
#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_GHOST 3

// Globals
extern uint8_t **game_map; // Current state of the map during gameplay
extern uint8_t **init_map; // Inital map to load on game start

void pacman_init();
void pacman_set_init_map(uint8_t** new_map);
void pacman_start();
void pacman_stop();
void pacman_next();

#endif /* PACMAN_H_ */
