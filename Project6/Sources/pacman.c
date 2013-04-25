/*
 * pacman.c
 *
 * Project 6
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "pacman.h"

// Globals
uint8_t **game_map;
uint8_t **init_map;
int player_x;
int player_y;
int player_dir;
int ghost_x;
int ghost_y;
int ghost_dir;
int ghost_move_tick;

// Initialize pacman constructs, should only be called once on program startup
void pacman_init() {
	// Allocate memory for game_map and init_map
	game_map = (uint8_t**)malloc(8*sizeof(uint8_t*));
	init_map = (uint8_t**)malloc(8*sizeof(uint8_t*));
	for(int i = 0; i < 8; i++) {
		init_map[i] = (uint8_t*)malloc(8*sizeof(uint8_t));
		game_map[i] = (uint8_t*)malloc(8*sizeof(uint8_t));
	}
	
	// Set initial map for the game
	uint8_t default_map[8][8] = {
	{ MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL },
	{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
	{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_PLAYER, MAP_WALL },
	{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
	{ MAP_WALL, MAP_EMPTY, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_EMPTY, MAP_WALL },
	{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
	{ MAP_WALL, MAP_GHOST, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
	{ MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL }
	};

	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			init_map[x][y] = default_map[x][y];
		}
	}
}

/*
 * Set the init map. Used when a new map is received over RS232 and
 */
void pacman_set_init_map(uint8_t** new_map) {
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			init_map[x][y] = new_map[x][y];
		}
	}
}

// Reset the pacman game state and start the game timer. Called every time a new game is created
void pacman_start() {
	// Copy init_map into game_map and find the initial player and ghost coordinates
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			game_map[x][y] = init_map[x][y];
			
			if(game_map[x][y] == MAP_PLAYER) {
				player_x = x;
				player_y = y;
			} else if(game_map[x][y] == MAP_GHOST) {
				ghost_x = x;
				ghost_y = y;
			}
		}
	}
	
	// Initial direction of the player and ghost
	player_dir = DIR_LEFT;
	ghost_dir = DIR_RIGHT;
	ghost_move_tick = 0;
	
	// Set input callback function
	nunchuk_set_input_callback(&pacman_input);
	
	// Start refresh timer
	led_display_game();
	pit0_init();
}

/*
 * Stop the pacman game
 */
void pacman_stop() {
	pit0_stop();
}

/*
 * Main game logic function called by the PIT0 timer
 * Calculates the next state of the game and prepares the game_map that will be transmitted to the LED
 */
void pacman_next() {
	
	// Move player
	pacman_next_player_move();
	if(pacman_is_game_over()) {
		pacman_stop();
		pacman_start();
		return;
	}
	
	// Move ghost
	/*pacman_next_ghost_move();
	if(pacman_is_game_over()) {
		pacman_stop();
		pacman_start();
		return;
	}*/

	// Finally, update the LED display pattern
	led_display_game();
}

/*
 * Determine if the player died and return true if the game is over
 */
int pacman_is_game_over() {
	return ((player_x == ghost_x) && (player_y == ghost_y));
}

/*
 * Returns true if the location is valid for the ghost or player to move to
 * The space must be within the bounds and not obstructed by a wall
 */
int pacman_is_valid_move_loc(int x, int y) {
	if(x < 0 || x > 7 || y < 0 || y > 7)
		return 0;
	
	if(game_map[x][y] == MAP_WALL)
		return 0;
	
	return 1;
}

/*
 * Calculate and update the players next movement
 */
void pacman_next_player_move() {
	int player_off_x = 0, player_off_y = 0;
	
	switch(player_dir) {
	case DIR_LEFT:
		player_off_x = -1;
		break;
	case DIR_RIGHT:
		player_off_x = 1;
		break;
	case DIR_UP:
		player_off_y = 1;
		break;
	case DIR_DOWN:
		player_off_y = -1;
		break;
	default:
		break;
	}
	
	// Don't move the player if the next pos is a wall
	if(pacman_is_valid_move_loc(player_x+player_off_x, player_y+player_off_y)) {
		// Change the players position and update it on the map
		game_map[player_x][player_y] = MAP_EMPTY;
		player_x += player_off_x;
		player_y += player_off_y;
		game_map[player_x][player_y] = MAP_PLAYER;
	}
}

/*
 * Calculate and update the ghosts next (random) movement
 */
void pacman_next_ghost_move() {
	int ghost_off_x = 0, ghost_off_y = 0;
	
	switch(ghost_dir) {
	case DIR_LEFT:
		ghost_off_x = -1;
		break;
	case DIR_RIGHT:
		ghost_off_x = 1;
		break;
	case DIR_UP:
		ghost_off_y = 1;
		break;
	case DIR_DOWN:
		ghost_off_y = -1;
		break;
	default:
		break;
	}
	
	if((ghost_move_tick < 5) && pacman_is_valid_move_loc(ghost_x+ghost_off_x, ghost_y+ghost_off_y)) {
		// Move the ghost if the next position isn't a wall AND less than 5 ticks have passed
		game_map[ghost_x][ghost_y] = MAP_EMPTY;
		ghost_x += ghost_off_x;
		ghost_y += ghost_off_y;
		game_map[ghost_x][ghost_y] = MAP_GHOST;
		
		ghost_move_tick++;
	} else {
		// Change the ghosts direction (randomly)
		ghost_dir = rand() % 4;
	}
}


/*
 * Input callback called from nunchuk_read()
 * Changes the direction of the player when the nunchuk stick changes direction
 */
void pacman_input(uint8_t input_cmd) {
	switch(input_cmd) {
	case NUNCHUK_INPUT_LEFT:
		player_dir = DIR_LEFT;
		break;
	case NUNCHUK_INPUT_RIGHT:
		player_dir = DIR_RIGHT;
		break;
	case NUNCHUK_INPUT_UP:
		player_dir = DIR_UP;
		break;
	case NUNCHUK_INPUT_DOWN:
		player_dir = DIR_DOWN;
		break;
	default:
		break;
	}
}
