/*
 * display.h
 *
 * Created: 17. 03. 2017 01:35:37
 *  Author: matja
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include "ugui/ugui.h"


void display_init (void); /* Initializes I2c and display */
void display_clear (void); /*Clears display */
void display_write_string (uint8_t row, uint8_t column, uint8_t *string); /*Writes string on dispay to location showd by row and column */
void pset(UG_U16 x, UG_U16 y, UG_COLOR c);
void display_update (void);

#define DISPLAY_ADR		0x3C

#define SSD1306_DEFAULT_ADDRESS 0x78
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_NOP 0xE3

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8
#endif /* DISPLAY_H_ */
