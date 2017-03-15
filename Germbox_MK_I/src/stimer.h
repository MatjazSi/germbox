/*
 * stimer.h
 *
 * Created: 27. 02. 2017 23:48:31
 *  Author: matja
 */ 


#ifndef STIMER_H_
#define STIMER_H_

#include <stdint.h>
#include "tc.h"


#define STIMER_NBR	5

typedef struct  
{
	uint32_t time;
	uint8_t running;
	uint8_t stopwatch; // if true timer is upcounting and callback function is disabled
	uint32_t auto_reload;
	volatile void (*function) (void);
}stimer_ch_t;

uint8_t autoreload;

void stimer_init (void);
uint32_t stimer_set_time(uint8_t timer, uint32_t t, uint8_t autoreload);
uint32_t stimer_start(uint8_t timer);
uint32_t stimer_stop (uint8_t timer);
uint32_t stimer_reset (uint8_t timer);
uint32_t stimer_get_time (uint8_t timer);
uint32_t stimer_set_as_stopwatch (uint8_t timer);
uint32_t stimer_set_as_timer (uint8_t timer);
uint32_t stimer_register_callback (uint8_t timer, void(*funct)(void));
uint32_t stimer_unregister_callback (uint8_t timer);
#endif /* STIMER_H_ */