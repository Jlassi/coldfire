/*
 * game.c
 *
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "game.h"

// Globals
uint8_t **pattern;
uint8_t g_paused;

void game_init() {	
	g_paused = 0;
	
	// Allocate memory for pattern
	pattern = (uint8_t**)malloc(8*sizeof(uint8_t*));
	for(int i = 0; i < 8; i++)
		pattern[i] = (uint8_t*)malloc(8*sizeof(uint8_t));
}

void game_start() {
	// Initial pattern for the game
	uint8_t init_pattern[8][8] = {
	{ 0, 0, 0, 1, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 1, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 1, 0 },
	{ 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 0, 1, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 1, 1, 0, 0, 0 }
	};
	
	// Copy init_pattern into pattern
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			pattern[x][y] = init_pattern[x][y];
		}
	}
	
	led_display_game();
}

// Checks if a given coordinate pair is in the game bounds
int game_in_bounds(int x, int y) {
	if(x < 0 || y < 0 || x > 8 || y > 8)
		return 0;
	return 1;
}

// Coordinate is in bounds and has an active piece
int game_alive(int x, int y) {
	return game_in_bounds(x, y) && (pattern[x][y] == 1);
}

int game_num_neighbours(int x, int y) {
	int ne = 0; // neighbours count
	int n_x = 0, n_y = 0; // current neighbour x and neighbour y we are looking at
	
	// Run through all possible neighbours to get a count for the number of neighbours
	n_x = x-1;
	n_y = y;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	n_x = x-1;
	n_y = y-1;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	n_x = x;
	n_y = y-1;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	n_x = x+1;
	n_y = y;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	n_x = x+1;
	n_y = y+1;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	n_x = x;
	n_y = y+1;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	n_x = x-1;
	n_y = y+1;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	n_x = x+1;
	n_y = y-1;
	ne += game_alive(n_x, n_y) ? 1 : 0;
	
	return ne;
}

// Calculate the next configuration of the game and display it
// Called by the PIT1 ISR
void game_next() {
	int ne = 0; // neighbour count
	uint8_t new_pattern[8][8];
	
	//printf("Game next\n");
	
	// Copy pattern into new_pattern
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			new_pattern[x][y] = pattern[x][y];
		}
	}
	
	// Operate game rules to create a new_pattern
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			ne = game_num_neighbours(x, y); // get new neighbour count
			
			if(game_alive(x, y)) { // Cell is live
				if(ne < 2) { // Fewer than 2 ne's, dies
					new_pattern[x][y] = 0;
				} else if(ne == 2 || ne == 3) { // 2 or 3 ne's, Lives on
				} else if(ne > 3) { // More than 3 ne's, dies
					new_pattern[x][y] = 0;
				}
			} else { // Dead cell
				if(ne == 3) // Exactly 3 neighbors, new live
					new_pattern[x][y] = 1;
			}
		}
	}
	
	// Copy new_pattern into pattern
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			pattern[x][y] = new_pattern[x][y];
		}
	}
	
	led_display_game();
}
