/*
 * display.h
 *
 * Created: 17. 03. 2017 01:35:37
 *  Author: matja
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#define DISPLAY_ADR		0x3C


void display_init (void);
void display_write_reg (uint8_t reg_addr, uint8_t value);
void display_write_buffer(uint8_t start_addr, uint8_t* buff, uint32_t len);
#endif /* DISPLAY_H_ */
