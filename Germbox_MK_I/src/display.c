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

twi_packet_t twiPacket;

void display_init (void)
{
	twi_options_t twiOptions;
	
	
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
	
	
}

void display_write_reg (uint8_t reg_addr, uint8_t value)
{
	twiPacket.addr[0] = reg_addr;
	twiPacket.addr_length = 1;
	twiPacket.buffer = &value;
	twiPacket.chip = DISPLAY_ADR;
	twiPacket.length = 1;
	twi_master_write(TWI0, &twiPacket);
}

void display_write_buffer(uint8_t start_addr, uint8_t* buff, uint32_t len)
{
	twiPacket.addr[0] = start_addr;
	twiPacket.addr_length = 1;
	twiPacket.buffer = buff;
	twiPacket.chip = DISPLAY_ADR;
	twiPacket.length = len;
	twi_master_write(TWI0, &twiPacket);
}