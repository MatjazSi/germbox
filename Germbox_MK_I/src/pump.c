/*
 * pump.c
 *
 * Created: 15. 03. 2017 13:37:30
 *  Author: matja
 */ 
#include "hardware_conf.h"
#include "pio.h"
#include "pmc.h"

void pump_init(void)
{
	pmc_enable_periph_clk(ID_PIOA);
	pio_set_output(PUMP_PORT, PUMP_PIN, LOW, DISABLE, DISABLE);
}