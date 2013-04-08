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

void game_init() {
	uint8_t init_pattern[8][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 }
	};
	
	// Allocate memory for pattern
	pattern = (uint8_t**)malloc(8*sizeof(uint8_t*));
	for(int i = 0; i < 8; i++)
		pattern[i] = (uint8_t*)malloc(8*sizeof(uint8_t));
	
	// Copy init_pattern into pattern
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			pattern[x][y] = init_pattern[x][y];
		}
	}
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
	
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			ne = game_num_neighbours(x, y); // get new neighbour count
			
			if(game_alive(x, y)) { // Cell is live
				if(ne < 2) { // Fewer than 2 ne's, dies
					pattern[x][y] = 0;
				} else if(ne == 2 || ne == 3) { // 2 or 3 ne's, Lives on
				} else { // More than 3 ne's, dies
					pattern[x][y] = 0;
				}
			} else { // Dead cell
				if(ne == 3) // Exactly 3 neighbors, new live
					pattern[x][y] = 1;
			}
		}
	}
	
	led_display_game();
}
