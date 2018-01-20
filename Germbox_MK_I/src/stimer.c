/*
 * stimer.c
 *
 * Created: 27. 02. 2017 23:47:42
 *  Author: matja
 */ 

#include "stimer.h"
#include "tc.h"
#include "pmc.h"
#include "sysclk.h"
#include <stdint.h>

stimer_ch_t stimer_times[STIMER_NBR];



void stimer_init (void)
{
	pmc_enable_periph_clk(ID_TC0);

	// Configure TC for a 1000Hz frequency and trigger on RC compare. 
	//tc_find_mck_divisor(1000, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 0, 250);

	/* Configure and enable interrupt on RC compare  */
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
	NVIC_DisableIRQ(TC0_IRQn);
	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_SetPriority(TC0_IRQn, 0);
	NVIC_EnableIRQ(TC0_IRQn);
	
	tc_start(TC0, 0);
}


void TC0_Handler(void)
{
	uint32_t n;
	uint32_t ul_dummy;
	
	// Clear status bit to acknowledge interrupt
	ul_dummy = tc_get_status(TC0, 0);
	for(n = 0; n < STIMER_NBR; n++)
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
	return 1;
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

