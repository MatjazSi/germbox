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

#include "asf.h""

#include "pid.h"
#include "stimer.h"
#include "display.h"
#include "heater.h"
#include "thermo.h"
#include "encoder.h"


#define PID_TIMER	0 // stimer channel number
#define PRINT_TIMER	1 // timer for printing things over usb


#define KP	45 //30
#define KI	0.07//0.004
#define KD	20		//8

#define  I_MAX		700

/* best so far (PI)
#define KP	90
#define KI	0.01
#define KD	0
*/

/* old settings (PID)

#define KP	150
#define KI	0.2
#define KD	6
*/

#define DEFAULT_SET_T	29.0

	volatile uint32_t time_expired = 0;
	volatile uint32_t ttp = 0;		// time to print

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

void time_to_print(void)
{
	ttp = 1;
}

int main (void)
{

	float temp, power, set_temp = DEFAULT_SET_T, old_set_temp = 0;
	uint8_t str_pos = 0;
	uint32_t whole, decimal;
	int32_t pulses;
	uint8_t str[20];
	pidstruct_t tPid;
	
	sysclk_init();
	board_init();
	
	udc_start();
	
	//init PID
	pid_init(TYPE_PID, KP, KI, KD, &tPid);
	pid_set_max_integral(I_MAX, &tPid);
	pid_set_limit(100.0, 0, &tPid);
	pid_anty_windup_enable(&tPid);
	
	
	//init timer that will trigger PID controller
	stimer_set_time(PID_TIMER, 200, 1);
	stimer_register_callback(PID_TIMER, pid_timeout);
	stimer_start(PID_TIMER);
	
	stimer_set_time(PRINT_TIMER, 1000, 1);
	stimer_register_callback(PRINT_TIMER, time_to_print);
	stimer_start(PRINT_TIMER);

	display_clear();
	while(1)
	{
		//get temperature
		temp = thermo_get_temp();
		if(time_expired) // time to execut pid controll loop
		{
			power = pid_execute(&tPid, (set_temp - temp));
			heater_set((uint8_t)power);
			time_expired = 0;
			
		}
		if(ttp) // time to print out things over USB
		{
			
			
			float_split(set_temp, &whole, &decimal);
			str_pos = sprintf(str, "$%2u.%1u", whole, decimal);
			float_split(temp, &whole, &decimal);
			str_pos += sprintf(str + (str_pos), ", %2u.%1u;", whole, decimal );
			udi_cdc_write_buf(str, str_pos);
			
			/*
			float_split(set_temp, &whole, &decimal);
			str_pos = sprintf(str, "$%2u.%1u", whole, decimal);
			float_split(temp, &whole, &decimal);
			str_pos += sprintf(str + (str_pos), ", %2u.%1u", whole, decimal );
			str_pos += sprintf(str + (str_pos), ", %d;", (uint32_t)tPid.sum );
			udi_cdc_write_buf(str, str_pos);
			*/
			ttp = 0;
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
		if(pulses)
		{
			pid_reset_int(&tPid);
		}
	}
	
}
