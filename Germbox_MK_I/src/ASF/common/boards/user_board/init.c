/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "wdt.h"

#include "heater.h"
#include "stimer.h"
#include "pump.h"
#include "display.h"
#include "thermo.h"
#include "encoder.h"

void board_init(void)
{
	wdt_disable(WDT);
	heater_init();
	pump_init();
	stimer_init();
	thermo_init();
	display_init();
	encoder_init();
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
}
