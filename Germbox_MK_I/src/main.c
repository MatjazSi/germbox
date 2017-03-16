/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdint.h>

#include "heater.h"
#include "stimer.h"
#include "pump"
#include "adc.h"
#include "wdt.h"

void tgl (void)
{
	volatile static n = 0;
	if(n)
	{
		pio_set_pin_group_low(PIOA, PIO_PA17);
		n = 0;
	}
	else
	{
		pio_set_pin_group_high(PIOA, PIO_PA17);
		n = 1;
	}
}


int main (void)
{

	
		
	
	sysclk_init();
	board_init();
	heater_init();

	pump_init();
	

	heater_set(32);

	stimer_init();
	
	pio_set_output(PIOA, PIO_PA17, LOW, DISABLE, DISABLE);
	
	stimer_set_time(0, 250, 1);
	stimer_register_callback(0, tgl);
	stimer_start(0);
	
	
	
	while(1)
	{

		asm("nop");
		asm("nop");
	}
	
	

	/* Insert application code here, after the board has been initialized. */
}
