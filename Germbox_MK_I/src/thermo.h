/*
 * thermo.h
 *
 * Created: 20. 03. 2017 00:52:16
 *  Author: matja
 */ 


#ifndef THERMO_H_
#define THERMO_H_


#define	GROUND_SENSOR_OFFSET	0.4				//offset in volts
#define GROUND_SENSOR_GAIN		1 / 10e-3		//sensor sensitivity in volts / °C
#define GROUND_SENSOR_ADC_GAIN	4.0
#define GROUND_SENSOR_ADC_OFS	0				//adc offset in LSB
#define REF_V					2.500			//reference voltage in volts
#define THERMO_AVERAGING		32				//number of measurments for averaging	

#define MOISTURE_AVERAGING				32		//number of measurents for averaging
#define MOISTURE_OFFSET					-3330	//moisture offset
#define MOISTURE_DIV_FACTOR				27		//division factor

void thermo_init(void);
float thermo_get_temp (void);
uint16_t thermo_get_moisture (void);

#endif /* THERMO_H_ */
