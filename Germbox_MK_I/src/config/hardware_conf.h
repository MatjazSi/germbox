/*
 * hardware_conf.h
 *
 * Created: 15. 03. 2017 12:54:53
 *  Author: matja
 */ 


#ifndef HARDWARE_CONF_H_
#define HARDWARE_CONF_H_

#include "pio.h"
#include "pwm.h"
#include "adc.h"

//Pump configuration
#define PUMP_PORT					PIOA
#define PUMP_PIN					PIO_PA15	

//Heater configuration
#define HEATER_PORT					PIOA
#define HEATER_PIN					PIO_PA16
#define HEATER_PIN_MUX				PIO_TYPE_PIO_PERIPH_C
#define HEATER_PWM_CH				PWM_CHANNEL_2

#define HEATER_PWM_FREQUENCY		50	//frequency in hertz

//Ground temp sensor config
#define GROUND_TEMP_PORT			PIOB
#define GROIND_TEMP_PIN				PIO_PB3

//Air temp  sensor config

//Ground moisture sensor config
#define GROUND_TEMP_PORT			PIOB
#define GROIND_TEMP_PIN				PIO_PB1

//Encoder config
#define ENC_PORT					PIOB
#define ENC_A_PIN					PIO_PA6
#define ENC_B_PIN					PIO_PA1




#endif /* HARDWARE_CONF_H_ */