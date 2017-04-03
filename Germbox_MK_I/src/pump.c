/**
 * @file pump.c
 * @author Siworks
 * @date 3 Apr 2017
 * @brief Water pump driving
 *
 * 
 */
#include "hardware_conf.h"
#include "pio.h"
#include "pmc.h"

/**
 * @brief Initiliazation of water pump
 *
 *	This functioon should be caled before using water pump
 */
void pump_init(void)
{
	pmc_enable_periph_clk(ID_PIOA);
	pio_set_output(PUMP_PORT, PUMP_PIN, LOW, DISABLE, DISABLE);
}