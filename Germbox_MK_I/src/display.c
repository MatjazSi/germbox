/*
 * display.c
 *
 * Created: 17. 03. 2017 01:35:00
 *  Author: matja
 */ 

#include "display.h"
#include "pio.h"
#include "twi.h"
#include "pmc.h"
#include <stdint.h>

uint8_t buff [10] = "Hello";

void display_init (void)
{
	twi_options_t twiOptions;
	twi_packet_t twiPacket;
	
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_TWI0);
	
	pio_set_peripheral(PIOA, PIO_TYPE_PIO_PERIPH_A, PIO_PA3);
	pio_set_peripheral(PIOA, PIO_TYPE_PIO_PERIPH_A, PIO_PA4);
	
	twiOptions.chip = DISPLAY_ADR;
	twiOptions.master_clk = 32000000;
	twiOptions.smbus = 0;
	twiOptions.speed = 100000;
	
	
	
	twi_enable_master_mode(TWI0);
	twi_master_init(TWI0, &twiOptions);
	
	//send test data
	
	while(1)
	{
		twiPacket.addr[0] = 0xCC;
		twiPacket.addr_length = 1;
		twiPacket.buffer = &buff;
		twiPacket.chip = DISPLAY_ADR;
		twiPacket.length = 5;
		twi_master_read(TWI0, &twiPacket);
		asm("nop");
		
	}
	
}