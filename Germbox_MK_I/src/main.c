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

#include "asf.h"

#include "pid.h"
#include "stimer.h"
#include "display.h"
#include "heater.h"
#include "thermo.h"
#include "pump.h"
#include "encoder.h"




#include "ugui/ugui.h"
#include "icones.h"
#include "graphic.h"

#define PID_TIMER		0 // stimer channel number
#define PRINT_TIMER		1 // timer for printing things over usb
#define MIN_TIMER		2 // timer used for minute counting
#define WATERING_TIMER	3 // timer used for measuring
#define DISPLAY_TIMER	4 // timer used for displaying of different things 


#define KP	35//55
#define KI	0.011//0.06
#define KD	100//15

#define  I_MAX 400

#define DEFAULT_SET_T			29.0	//default set temperature  in °C
#define DEFAULT_SET_HUMID		3		//defeaut set humidity
#define DEFAULT_WATERING_TIME	10		//default duration of watering in seconds

#define WATTERING_INTERVAL		120 //watering interval in minutes
#define SHORT_WATERING			15 //watering duration in seconds
#define LONG_WATERING			25 //watering duration in seconds
#define HUMID_TRESHOLD			4 // treshild between short and long wattering duration

#define SET_POINT_DISPALY_TIME	1500 // time of dispaly overriding in ms


//#define PRINT_OVER_USB //uncoment if you want controller to prnt data out over USB


volatile uint32_t time_expired = 0, watering_expired = 0; // indicator flags
volatile uint32_t ttp = 0;		// time to print indicator flag

volatile uint32_t expired_time = 0;  //global timer variable
volatile uint32_t water_timer = 0;	//watering timer vaariable

volatile uint32_t display_show_set = 0;

GRprogressbar_t bar1;

//counts time from the start of program
void min_count (void)
{
	water_timer++;
	expired_time++;
	
	if(water_timer == WATTERING_INTERVAL)
	{
		water_timer = 0;
		watering_expired = 1;
	}
}
// callback function taht is called when watering duration  is over and pump should be turned off
static void water_off (void)
{
	pump_off;
}

//initialization of minute counter used for timing
static void init_min_counter (void)
{
	stimer_set_time(MIN_TIMER, 60000, 1);
	stimer_register_callback(MIN_TIMER, min_count);
	stimer_start(MIN_TIMER);
}

//initialization of watering timer used for watering duration timeing
static void init_watering_timer (void)
{
	stimer_set_time(WATERING_TIMER, DEFAULT_WATERING_TIME * 100, 0);
	stimer_register_callback(WATERING_TIMER, water_off);
}

// function starts watering for time given in argumenz (in seconds)
void water (uint8_t amount)
{
	stimer_set_time(WATERING_TIMER, amount * 1000, 0);
	pump_on; // pump will be turned off in callback routine striggered automaticaly by timer
	stimer_start(WATERING_TIMER);
	
}

// Auxilary function to split float to integral and decimal part
static void float_split (float input, uint32_t* wh, uint32_t* dec)
{
	uint32_t temp;
	temp = (uint32_t)input;
	*dec =((input -(float)temp) * 10);
	*wh = (uint32_t)temp;
}

// Callback function witch is called whenever PID loop timer has expired
void pid_timeout(void)
{
	time_expired = 1;
}
// Callback function witch is called whenever USB printout timer has expired
void time_to_print(void)
{
	ttp = 1;
}

// Callback function to change deispalying of set values
void display_set_point (void)
{
	stimer_stop(DISPLAY_TIMER);
	stimer_set_time(DISPLAY_TIMER, SET_POINT_DISPALY_TIME, 0);
	display_show_set = 0;
}

int main (void)
{

	float temp, power, set_temp = DEFAULT_SET_T;					// Heatiing related variables
	uint8_t str_pos = 0;											// USB text printing variaable
	uint8_t humid;													// Current humidity
	uint8_t old_pb = 1, pb = 0;										// Push button related variables
	uint8_t chars = 0;												// Printing related variables
	uint8_t days, hrs, mins;										// Time printout variables
	int8_t set_humid = DEFAULT_SET_HUMID;							// Set humidity
	volatile uint8_t selector = 0;									// selector 0 -> seting temperature, selector -> setting humidity
	uint32_t whole, decimal;										// variables used for printing floats
	int32_t pulses;													// variable used for encoder handling
	uint8_t str[20];												// buffer for text printing
	pidstruct_t tPid;												// Temperature controller PID structure
	GRprogressbar_t PB_temp, PB_humid, PB_pow;
	
	
	sysclk_init();
	board_init();
	init_min_counter(); // start all timers
	init_watering_timer();
	
	#ifdef PRINT_OVER_USB	
		udc_start();
	#endif
	
	//init PID
	pid_init(TYPE_PID, KP, KI, KD, &tPid);
	pid_set_max_integral(I_MAX, &tPid);
	pid_set_limit(100.0, 0, &tPid);
	pid_anty_windup_enable(&tPid);
	tPid.cycyles_count = 0;
	
	
	//init timer that will trigger PID controller
	stimer_set_time(PID_TIMER, 200, 1);
	stimer_register_callback(PID_TIMER, pid_timeout);
	stimer_start(PID_TIMER);
	
	//init timer for printout over USB
	stimer_set_time(PRINT_TIMER, 1000, 1);
	stimer_register_callback(PRINT_TIMER, time_to_print);
	stimer_start(PRINT_TIMER);
	
	//init objects on display
	UG_FontSelect(&FONT_6X8);
	GR_ProgressBar_create(&PB_temp, 15, 5, 8, 70, 100, 400);
	GR_ProgressBar_create(&PB_humid, 15, 20, 8, 70, 0, 10);
	GR_ProgressBar_create(&PB_pow, 15, 32, 4, 70, 0, 100);
	GR_Carrot_create(&PB_temp, CARRIOT_SINGLE, 3);
	GR_Carrot_create(&PB_humid, CARRIOT_SINGLE, 3);
	GR_ProgressBar_init(&PB_temp);
	GR_ProgressBar_init(&PB_humid);
	GR_ProgressBar_init(&PB_pow);
	UG_PutString(0, 5, "T:");
	UG_PutString(0, 20, "H:");
	UG_PutString(0, 32, "P:");
	
	stimer_set_time(DISPLAY_TIMER, SET_POINT_DISPALY_TIME, 0);
	stimer_register_callback(DISPLAY_TIMER, display_set_point);

	display_clear();
	display_update();

	//initial watering to purge all air from hose
	water(10);
	
	while(1)
	{
		//start of control loop **************************************************************
		//get temperature and humidity
		temp = thermo_get_temp();
		humid = thermo_get_moisture();
		if(time_expired) // time to execut pid controll loop
		{
			power = pid_execute(&tPid, (set_temp - temp));
			heater_set((uint8_t)power);
			time_expired = 0;
			
		}
		if(watering_expired)  // time to check if wattering is needed
		{
			if(set_humid)
			{
				if(humid < set_humid)
				{
					if((int8_t)(set_humid - humid) < HUMID_TRESHOLD)
					{
						water(SHORT_WATERING);
					}
					else
					{
						water(LONG_WATERING);
					}
				}
			}
			watering_expired = 0;	
		}
		//end of control loop **********************************************************
		
		#ifdef PRINT_OVER_USB
		if(ttp) // time to print out things over USB
		{
			
			
			float_split(set_temp, &whole, &decimal);
			str_pos = sprintf(str, "$%2u.%1u", whole, decimal);
			float_split(temp, &whole, &decimal);
			str_pos += sprintf(str + (str_pos), ", %2u.%1u;", whole, decimal );
			udi_cdc_write_buf(str, str_pos);
			ttp = 0;
		}
		#endif
		//dispaly handling
		
		if(!display_show_set) // clear background of satpoint print location
		{
			UG_FillFrame(10, 40, 118, 60, 0);
		}
		
		days = expired_time / (24*60);
		hrs = (expired_time - (60 * 24 * days)) / 60;
		mins = (expired_time - (60 * 24 * days)) % 60;
		
		float_split(set_temp, &whole, &decimal);
		GR_Carrot_update(&PB_temp, whole * 10 + decimal);
		//if you decide to put in visual indication of value being changed, you do it here*************************************
		if(selector)
		{
		//	sprintf(str + chars, " ");
		}
		else
		{
			//sprintf(str + chars, "<");
		}
		//end of visual indication ************************************
		GR_Carrot_update(&PB_humid, set_humid);
		GR_ProgressBar_update(&PB_humid, humid);
		sprintf(str, "  %2u", humid);
		UG_PutString(90, 20, str);
		
		//display_write_string(1, 0, str);
		
		float_split(temp, &whole, &decimal);
		sprintf(str, "%2u.%1uC", whole, decimal);
		UG_PutString(90, 5, str);
		GR_ProgressBar_update(&PB_temp, whole * 10 + decimal);
		
		GR_ProgressBar_update(&PB_pow, power);
		
		sprintf(str, "DUR:%2ud:%02uh:%02um", days, hrs, mins);
		UG_PutString(10, 42, str);
		
		sprintf(str, "NEXT:%2uh:%02um", (WATTERING_INTERVAL - water_timer) / 60,  (WATTERING_INTERVAL - water_timer) % 60);
		UG_PutString(10, 52, str);
		
		if(display_show_set)
		{
			
			UG_FillFrame(10, 40, 118, 60, 1);
			UG_FillFrame(12, 42, 116, 58, 0);
			if(!selector)
			{
				UG_FontSelect(&FONT_7X12);
				float_split(set_temp, &whole, &decimal);
				sprintf(str, "SET T:%2u.%1uC", whole, decimal);
				UG_PutString(20, 46, str);
				UG_FontSelect(&FONT_6X8);
			}
			else
			{
				UG_FontSelect(&FONT_7X12);
				sprintf(str, "SET H:%2u", set_humid);
				UG_PutString(20, 46, str);
				UG_FontSelect(&FONT_6X8);
			}
		}
		
		
		
		/*
		display_write_string(2, 0, str);
		
		sprintf(str, "PWR:%3u ", (uint32_t)power);
		display_write_string(3, 0, str);
		
		sprintf(str, "DURATION:%2ud:%02uh:%02um", days, hrs, mins);
		display_write_string(4, 0, str);
		
		sprintf(str, "NEXT:%2uh:%02um", (WATTERING_INTERVAL - water_timer) / 60,  (WATTERING_INTERVAL - water_timer) % 60);
		display_write_string(5, 0, str);
		*/
		display_update();
		//encoder handling
		pulses = encoder_get();
		if(pulses)
		{
			display_show_set = 1;
			stimer_set_time(DISPLAY_TIMER, SET_POINT_DISPALY_TIME, 0);
			stimer_start(DISPLAY_TIMER);
			if(selector)
			{
				set_humid += pulses;
				if(set_humid > 10)
				{
					set_humid = 10;
				}
				else if (set_humid < 0)
				{
					set_humid = 0;
				}
			}
			else
			{
				set_temp += (pulses * 0.5);
				if(set_temp > 40.0)
				{
					set_temp = 40.0;
				}
				else if (set_temp < 10.0)
				{
					set_temp = 10.0;
				}
			}
		}
		
		
		if(pulses && (!selector))
		{
			pid_reset_int(&tPid);
		}
		
		pb = encoder_get_pb();
		if(pb != old_pb)
		{
			display_show_set = 1;
			stimer_set_time(DISPLAY_TIMER, SET_POINT_DISPALY_TIME, 0);
			stimer_start(DISPLAY_TIMER);
			old_pb = pb;
			if(!pb)
			{
				selector++;
				if(selector > 1)
				{
					selector = 0;
				}	
			}
			
		}
	}
	
}
