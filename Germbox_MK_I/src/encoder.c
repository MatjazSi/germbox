/*
 * encoder.c
 *
 * Created: 19. 03. 2017 23:51:47
 *  Author: matja
 */ 
#include "pio.h"
#include "pmc.h"

#include "hardware_conf.h"


void encoder_init (void)
{
	pmc_enable_periph_clk(ID_PIOA);
	pio_set_input(ENC_PORT, ENC_A_PIN, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_input(ENC_PORT, ENC_B_PIN, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_input(ENC_PORT, ENC_SW_PIN, PIO_PULLUP | PIO_DEBOUNCE);
}