/*
 * display.c
 *
 * Created: 17. 03. 2017 01:35:00
 *  Author: matja
 */ 

#include "display.h"
#include "u8g2.h"

#include "pio.h"
#include "twi.h"
#include "pmc.h"
#include <stdint.h>

twi_packet_t twiPacket;
u8g2_t display;

static void display_write_reg (uint8_t reg_addr, uint8_t value)
{
	twiPacket.addr[0] = reg_addr;
	twiPacket.addr_length = 1;
	twiPacket.buffer = &value;
	twiPacket.chip = DISPLAY_ADR;
	twiPacket.length = 1;
	twi_master_write(TWI0, &twiPacket);
}

static void display_write_buffer(uint8_t start_addr, uint8_t* buff, uint32_t len)
{
	twiPacket.addr[0] = start_addr;
	twiPacket.addr_length = 1;
	twiPacket.buffer = buff;
	twiPacket.chip = DISPLAY_ADR;
	twiPacket.length = len;
	twi_master_write(TWI0, &twiPacket);
}

static void display_comand (uint8_t com)
{
	display_write_reg(0, com);
}

void display_data (uint8_t data)
{
	display_write_reg(64, data);
}

static void twi_init (void)
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

static void display_select_column (uint8_t col)
{
	if(col > 128)
	{
		col = 128;
	}
	display_comand(0x0F & (uint8_t)col);
	display_comand(0x10 | (col >> 4));
}

static void display_select_row (uint8_t row)
{
	if(row > 8)
	{
		row = 8;
	}
	display_comand(0xB0 | row);
}

void display_clear (void)
{
	uint8_t page, col;
	for(page = 0; page < 8; page++)
	{
		display_select_row(page);
		display_select_column(0);
		for(col = 0; col < 128; col++)
		{
			display_data(0x00);
		}
		
	}
}

void display_init (void)
{
	
	twi_init();
	display_comand(SSD1306_DISPLAYOFF);

	display_comand(SSD1306_SETDISPLAYCLOCKDIV);
	display_comand(0x80);

	display_comand(SSD1306_SETMULTIPLEX);
	display_comand(0x3F);
	
	display_comand(SSD1306_SETDISPLAYOFFSET);
	display_comand(0x00);
	
	display_comand(SSD1306_SETSTARTLINE | 0x00);
	
	// We use internal charge pump
	display_comand(SSD1306_CHARGEPUMP);
	display_comand(0x14);
	
	// Horizontal memory mode
	display_comand(SSD1306_MEMORYMODE);
	display_comand(0x00);
	
	display_comand(SSD1306_SEGREMAP | 0x1);

	display_comand(SSD1306_COMSCANDEC);

	display_comand(SSD1306_SETCOMPINS);
	display_comand(0x12);

	// Max contrast
	display_comand(SSD1306_SETCONTRAST);
	display_comand(0xCF);

	display_comand(SSD1306_SETPRECHARGE);
	display_comand(0xF1);

	display_comand(SSD1306_SETVCOMDETECT);
	display_comand(0x40);

	display_comand(SSD1306_DISPLAYALLON_RESUME);

	// Non-inverted display
	display_comand(SSD1306_NORMALDISPLAY);

	// Turn display back on
	display_comand(SSD1306_DISPLAYON);
	
	
	display_clear();
	display_select_row(3);
	display_select_column(17);
	for(uint32_t n = 0; n < 20; n++)
	{
		
		display_data(0XFF);
	}
	display_select_row(4);
	display_select_column(18);
	for( uint32_t n = 0; n < 20; n++)
		{
			
			display_data(0XFF);
		}
}




