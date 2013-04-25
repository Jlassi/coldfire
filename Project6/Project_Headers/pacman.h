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

// Direction types
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN 3

// Globals
extern uint8_t **game_map; // Current state of the map during gameplay
extern uint8_t **init_map; // Inital map to load on game start

extern int player_x;
extern int player_y;
extern int player_dir;
extern int ghost_x;
extern int ghost_y;

void pacman_init();
void pacman_set_init_map(uint8_t** new_map);
void pacman_start();
void pacman_stop();
void pacman_next();
void pacman_input(uint8_t input_cmd);

#endif /* PACMAN_H_ */
