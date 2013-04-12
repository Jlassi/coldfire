/*
 * pacman.h
 *
 * Project 6
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef PACMAN_H_
#define PACMAN_H_

// Map tile types
#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_PLAYER 2
#define MAP_GHOST 3

// Globals
extern uint8_t **game_map;

void pacman_init();
void pacman_reset();

#endif /* PACMAN_H_ */
