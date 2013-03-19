/*
 * main implementation: use this sample to create your own application
 *
 */


#include "support_common.h" /* include peripheral declarations and more */

#include "gpio.h"
#include "gpt.h"
#include "pit.h"
#include "pwm.h"
#include "note.h"
#include "song.h"
#include "uc_led.h"


#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

extern uint32 __VECTOR_RAM[];

static void init();

static void init() {
	asm_set_ipl(0); // Don't mask any levels
	note_init();
	song_init();
	gpt_port_ta_init(); // button init
	gpio_port_ta_init(); // speaker gpio init
	pwm_init(); // speaker init
	uc_led_init();
	uc_led_all_off();
	pit_init();
}

__declspec(noreturn) int main(void)
{
	init();
	
	while(1) {
	}
}
