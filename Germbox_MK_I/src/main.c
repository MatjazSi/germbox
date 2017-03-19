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
#include "pump.h"
#include "display.h"

#include "adc.h"
#include "wdt.h"


int main (void)
{

	
	uint8_t bfr [] = "St. patrick's day";
	
	sysclk_init();
	board_init();
	heater_init();
	pump_init();
	stimer_init();

	
	display_init();
	display_write_string(0, 0, bfr);
	while(1)
	{

	}
	
}
