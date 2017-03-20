/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdint.h>
#include <stdio.h>

#include "pid.h"
#include "stimer.h"
#include "display.h"
#include "heater.h"
#include "thermo.h"
#include "encoder.h"

#define PID_TIMER	0 // stimer channel number


#define KP	30
#define KI	0.1
#define KD	0.0

#define DEFAULT_SET_T	29.0

	volatile uint32_t time_expired = 0;

void float_split (float input, uint32_t* wh, uint32_t* dec)
{
	uint32_t temp;
	temp = (uint32_t)input;
	*dec =((input -(float)temp) * 10);
	*wh = (uint32_t)temp;
}

void pid_timeout(void)
{
	time_expired = 1;
}

int main (void)
{

	float temp, power, set_temp = DEFAULT_SET_T;
	uint32_t whole, decimal;
	int32_t pulses;
	uint8_t str[20];
	pidstruct_t tPid;
	
	sysclk_init();
	board_init();
	
	//init PID
	tPid.cP = KP;
	tPid.cI = KI;
	tPid.cD = KD;
	tPid.type = TYPE_PID;
	tPid.maxIntegrall = 2500;
	tPid.limit = 100;
	
	//init timer that will trigger PID controller
	stimer_set_time(PID_TIMER, 1000, 1);
	stimer_register_callback(PID_TIMER, pid_timeout);
	stimer_start(PID_TIMER);

	display_clear();
	while(1)
	{
		//get temperature
		temp = thermo_get_temp();
		if(time_expired) // time to eecut pid controll loop
		{
			power = pid_execute(&tPid, (set_temp - temp));
			if(power < 0)
			{
				power = 0;
			}
			heater_set((uint8_t)power);
			time_expired = 0;
		}
		//dispaly handling
		float_split(set_temp, &whole, &decimal);
		sprintf(str, "SET:%2u.%1u °C", whole, decimal);
		display_write_string(0, 0, str);
		
		float_split(temp, &whole, &decimal);
		sprintf(str, "TMP:%2u.%1u °C", whole, decimal);
		display_write_string(1, 0, str);
		
		sprintf(str, "PWR:%3u ", (uint32_t)power);
		display_write_string(2, 0, str);
		//encoder handling
		pulses = encoder_get();
		set_temp += (pulses * 0.5);
	}
	
}
