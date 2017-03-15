/*
 * stimer.c
 *
 * Created: 27. 02. 2017 23:47:42
 *  Author: matja
 */ 

#include "stimer.h"
#include "tc.h"
#include "tc_interrupt.h"
#include "led.h"
#include <stdint.h>
stimer_ch_t stimer_times[STIMER_NBR];
struct tc_module tc_instance;


void stimer_init (void)
{
	struct tc_config tc_conf;
	uint8_t n;
	
	for(n = 0; n < (STIMER_NBR - 1); n++) // init stimer struct
	{
		stimer_times[n].auto_reload = 0;
		stimer_times[n].function = 0;
		stimer_times[n].running = 0;
		stimer_times[n].stopwatch = 0;
		stimer_times[n].time = 0;	
	}
	
	tc_get_config_defaults(&tc_conf);
	tc_conf.clock_source = GCLK_GENERATOR_1;
	tc_conf.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	tc_conf.count_direction = TC_COUNT_DIRECTION_UP;
	tc_conf.counter_size = TC_COUNTER_SIZE_16BIT;
	tc_conf.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	
	tc_init(&tc_instance, TC3, &tc_conf);
	
	tc_register_callback(&tc_instance, stimer_deamon, TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
	
	tc_enable(&tc_instance);
	tc_set_top_value(&tc_instance, 33);
	tc_start_counter(&tc_instance);
	system_interrupt_enable_global();
}


void stimer_deamon (struct tc_module *const module)
{
	uint32_t n;
	for(n = 0; n < (STIMER_NBR -1); n++)
	{
		if(stimer_times[n].running)
		{
			if(stimer_times[n].stopwatch) // upcounting mode
			{
				stimer_times[n].time++;
			}
			else
			{
				stimer_times[n].time--;
				if(!stimer_times[n].time) // timer has expired
				{
					if(stimer_times[n].function)
					{
						stimer_times[n].function(); // call callback function
					}
					if(stimer_times[n].auto_reload)
					{
						stimer_times[n].time = stimer_times[n].auto_reload;
					}
					else
					{
						stimer_times[n].running = 0;
					}
				}
			}
		}
	}
}


uint32_t stimer_set_time(uint8_t timer, uint32_t t, uint8_t autoreload)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].time = t;
	if(autoreload)
	{
		stimer_times[timer].auto_reload = t;
	}
	else
	{
		stimer_times[timer].auto_reload = 0;
	}
	return 1;
}

uint32_t stimer_start(uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	if(!stimer_times[timer].running) //check that timer is not running yet
	{
		if(!stimer_times[timer].stopwatch) // downcounting mode
		{
			if(!stimer_times[timer].time)
			{
				return 0;
			}
		}
		stimer_times[timer].running = 1;
	}
}

uint32_t stimer_stop (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].running = 0;
	return 1;
}

uint32_t stimer_reset (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].time = 0;
	return 1;
	
}

uint32_t stimer_get_time (uint8_t timer)
{
	return stimer_times[timer].time;
}

uint32_t stimer_set_as_stopwatch (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].stopwatch = 1;
	return 1;
}

uint32_t stimer_set_as_timer (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].stopwatch = 0;
	return 1;
}

uint32_t stimer_register_callback (uint8_t timer, void(*funct)(void))
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].function = funct;
	return 1;
}

uint32_t stimer_unregister_callback (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].function = 0;
	return 1;
}

