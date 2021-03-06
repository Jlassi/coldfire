/*
 * led.c
 *
 * Project 6
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "led.h"

// Globals
uint8_t led_current_pattern[8][8];
uint8_t g_green[8];
uint8_t g_red[8];
uint8_t g_row;

// Inits the LED matrix
void led_init() {
	MCF_GPIO_PTHPAR &= ~MCF_GPIO_PTHPAR_PTHPAR6(3); // GPIO TH pin 6 for GPIO function, LEDM_STR
	MCF_GPIO_PTHPAR &= ~MCF_GPIO_PTHPAR_PTHPAR7(3); // GPIO TH pin 7 for GPIO function, LEDM_OE
	
	MCF_GPIO_DDRTH |= MCF_GPIO_DDRTH_DDRTH6 | MCF_GPIO_DDRTH_DDRTH7;
	
	// OE high. Disables outputs
	MCF_GPIO_SETTH |= MCF_GPIO_SETTH_SETTH7;
	
	// Initialize QSPI to transfer at 5000kbps with a delay of 0 (default)
	qspi_init(5000, 0);
}

// Display a blank output on the LED (useful for modes outside of the game mode)
void led_display_blank() {
	for(int y = 0; y < 8; y++) {
		for(int x = 0; x < 8; x++) {
			led_current_pattern[y][x] = BLACK;
		}
	}
	
	led_write(&led_current_pattern);
}

// Copy the game's map to the LED pattern
void led_display_game() {
	// Convert game_map tiles into appropriate colored tiles and set into led_current_pattern
	for(int y = 0; y < 8; y++) {
		for(int x = 0; x < 8; x++) {
			if(game_map[y][x] == MAP_WALL)
				led_current_pattern[y][x] = GREEN;
			else if(game_map[y][x] == MAP_PLAYER)
				led_current_pattern[y][x] = ORANGE;
			else if(game_map[y][x] == MAP_GHOST)
				led_current_pattern[y][x] = RED;
			else {
				led_current_pattern[y][x] = BLACK; // MAP_EMPTY
			}
		}
	}
	
	led_write(&led_current_pattern);
}

// Refreshes the matrix by writing the next row
void led_refresh() {
	led_write_row(); // writes the next row
	g_row++;
	if(g_row == 8) {
		g_row = 0;
	}
}

// Write a specific 8x8 pattern
void led_write(uint8_t (*pattern)[8][8]) {
	for(int row = 0; row <= 7; row++) {
		for(int col = 0; col <= 7; col++) {
			if((*pattern)[row][col] == RED) {
				g_red[row] |= (1 << col);
			} else if((*pattern)[row][col] == GREEN) {
				g_green[row] |= (1 << col);
			} else if((*pattern)[row][col] == ORANGE) {
				g_red[row] |= (1 << col);
				g_green[row] |= (1 << col);
			} else if((*pattern)[row][col] == BLACK) {
				g_green[row] &= ~(1 << col);
				g_red[row] &= ~(1 << col);
			} else {
				// Unknown color. shouldn't happen
			}
		}
	}
	g_row = 0;
}

// Uses QSPI to transmit 3 bytes to turn on LEDs for current g_row
void led_write_row() {
	uint8_t *data = (uint8_t*)malloc(3 * sizeof(uint8_t));
	data[0] = g_red[g_row];
	data[1] = g_green[g_row];
	data[2] = ~(1 << g_row);
	
	// OE high. Disables outputs
	MCF_GPIO_SETTH |= MCF_GPIO_SETTH_SETTH7;
	
	// STR low (must write 0 to clrth)
	MCF_GPIO_CLRTH &= ~(MCF_GPIO_SETTH_SETTH6);

	qspi_send(data, 3);

	// STR low to high will transfer bits in shift registers to storage registers
	MCF_GPIO_SETTH |= MCF_GPIO_SETTH_SETTH6;
	
	// OE low. Enables output
	MCF_GPIO_CLRTH &= ~(MCF_GPIO_SETTH_SETTH7);

	
	free(data);
}
