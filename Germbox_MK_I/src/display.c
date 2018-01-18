/**
 * @file display.c
 * @author Siworks
 * @date 3 Apr 2017
 * @brief Basic Oled display routines for SSD1603
 *
 * 
 *  Dipslay usage is limited to text only. The only supported font is 5X7.
 *	Text must be aligned to Page number
 * 
 * 
 */


#include "display.h"
#include "font.h"
#include "ugui/ugui.h"

#include "pio.h"
#include "twi.h"
#include "pmc.h"
#include <stdint.h>

twi_packet_t twiPacket;
UG_GUI Gui;
uint8_t display_buffer[128 * 64 / 8]; // 128 * 8 display resolution 



/**
 * @brief Writes data do dispaly registers
 *
 * @param reg_addr Register addres
 * @param value Value to write to register
 */
static void display_write_reg (uint8_t reg_addr, uint8_t value)
{
	twiPacket.addr[0] = reg_addr;
	twiPacket.addr_length = 1;
	twiPacket.buffer = &value;
	twiPacket.chip = DISPLAY_ADR;
	twiPacket.length = 1;
	twi_master_write(TWI0, &twiPacket);
}

/**
 * @brief Writes comand to display
 *
 * @param com Comand number
 */
static void display_comand (uint8_t com)
{
	display_write_reg(0, com);
}

/**
 * @brief Writes data to display
 *
 * @param data Data to be written
 */
void display_data (uint8_t data)
{
	display_write_reg(64, data);
}

/**
 * @brief Initiates TWI interface in microcontroller
 *
 */
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
	twiOptions.speed = 400000;
	
	twi_enable_master_mode(TWI0);
	twi_master_init(TWI0, &twiOptions);
	
	
	
}
/**
 * @brief Selects a column to write to
 *
 * @param col Selected column
 */
static void display_select_column (uint8_t col)
{
	if(col > 128)
	{
		col = 128;
	}
	display_comand(0x0F & (uint8_t)col);
	display_comand(0x10 | (col >> 4));
}

/**
 * @brief Selects a row to write to
 *
 * @param row Selected row
 */
static void display_select_row (uint8_t row)
{
	if(row > 8)
	{
		row = 8;
	}
	display_comand(0xB0 | row);
}

/**
 * @brief Writes a character to display
 *
 * Characters are stored in font.h file
 *
 * @param ch Character to write to dispaly
 */
static void display_putch (uint8_t ch)
{
	uint32_t offset = 0, sub_offset;
	if(ch < 32)
	{
		ch = 32;
	}
	ch -= 32;
	offset = ch * 5;
	for(sub_offset = 0; sub_offset < 5; sub_offset++)
	{
		display_data(font[offset + sub_offset]);
	}
	display_data(0x00);
	
}

void pset(UG_U16 x, UG_U16 y, UG_COLOR c)
{
	unsigned int i,p;

	if ( x > 127 ) return;
	p = y>>3; // :8
	p = p<<7; // *128
	p +=x;

	if( c )
	{
		display_buffer[p] |= 1 << (y % 8);
	}
	else
	{
		display_buffer[p] &= ~(1 << (y % 8));
	}

}

/**
 * @brief Clear the dispaly
 *
 */
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

/**
 * @brief Initializes the display
 *
 * This function should be called before using dispaly
 */
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
	UG_Init(&Gui, pset, 128, 64);
	

}

/**
 * @brief Writes an ASCII string to display 
 *
 * String should be zero terminated.
 *
 * @param row Row to write the text to
 * @param column Start column
 * @param *string String to be written
 */
void display_write_string (uint8_t row, uint8_t column, uint8_t *string)
{
	display_select_row(row);
	display_select_column(column);
	while(*string)
	{
		display_putch(*string++);
	}
}


void display_update (void)
{
	uint32_t n;
	display_select_column(0);
	display_select_row(0);
	for(n = 0; n < (128 * 64 / 8); n++)
	{
		display_data(display_buffer[n]);
	}
}

void display_draw_image (uint32_t x_size, uint32_t y_size, uint8_t *img)
{
	uint32_t x, y, n, byte;
	for(y = 0; y < y_size; y++)
	{
		for(x = 0; x < x_size; x++)
		{
			byte = ((y * (x_size )) + x) / 8;
			n = x % 8;
			if((*(img + byte)) & (1 <<  7 - n))
			{
				pset(x, y, 1);
			}
			else
			{
				pset(x, y, 0);
			}
		}
	}
	
}




