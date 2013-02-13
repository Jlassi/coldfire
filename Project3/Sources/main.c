/*
 * main implementation: use this sample to create your own application
 *
 */


#include "support_common.h" /* include peripheral declarations and more */
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

static int dipsw_delay_poll();
static void dipsw_onoff_poll();
static void flash_led_sequence(int p_delay);
static void init();


static int dipsw_delay_poll(){
	
}

static void dipsw_onoff_poll(){
	
}

static void flash_led_sequence(int p_delay){
	
}

static void init(){
	
}

int main(void)
{
	int counter = 0;

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n\r");
#endif
	for(;;) {	   
	   	counter++;
	}
}
