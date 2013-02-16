/*
 * main implementation: use this sample to create your own application
 *
 */


#include "support_common.h" /* include peripheral declarations and more */
#include "uc_dipsw.h"
#include "uc_led.h"
#include "dtim.h"
#include "gpio.h"
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

static int reverse;

static int dipsw_delay_poll();
static void dipsw_onoff_poll();
static void flash_led_sequence(int p_delay);
static void init();

/**
 * Polls DIP switches 3, and 4 to see what the delay should be set to. Returns the delay in ms.
 */
static int dipsw_delay_poll() {
	int delay = 250;
	
	// Switch 3 in the On position adds 200ms
	if(uc_dipsw_get_state(3) == 1)
		delay += 200;
	
	// Switch 4 in the On position adds 100ms
	if(uc_dipsw_get_state(4) == 1)
		delay += 100;
	
	return delay;
}

/**
 * Polls DIP switch 2 to see if the program should be reversed because it has been moved to the 
 * Off position or if the program should go back to its original direction because it has been 
 * moved to the On position.
 */
static void dipsw_onoff_poll() {
	if(uc_dipsw_get_state(2) == 1)
		reverse = 0;
	else
		reverse = 1;
}

/**
 * Flash the LED's in the proper sequence per the project requirements
 */
static void flash_led_sequence(int p_delay) {
	int i = 0;
	
	if(reverse)
		i = 4;
	else
		i = 1;
	
	while((i > 0) && (i <= 4)) {
		uc_led_on(i);
		dtim0_delay(p_delay);
		uc_led_off(i);
		dtim0_delay(p_delay);
		
		if(reverse)
			i--;
		else
			i++;
		
		// Poll for dipsw status to determine if we are going in reverse order or not
		dipsw_onoff_poll();
	}
}

/**
 * Calls uc_dipsw_init() to initialize the DIP switch, uc_led_init() to initialize the LED's,
 * and dtim0_init() to initialize DTIM0.
 */
static void init() { 
	uc_dipsw_init();
	uc_led_init();
	dtim0_init();
}

__declspec(noreturn) int main() {
	// Initialize hardware
	init();
	uc_led_all_off();
	
	reverse = 1;
	
	// Main program logic
	while(1) {
		flash_led_sequence(dipsw_delay_poll());
	}
}
