/*
 * thermo.h
 *
 * Created: 20. 03. 2017 00:52:16
 *  Author: matja
 */ 


#ifndef THERMO_H_
#define THERMO_H_



void thermo_init(void);
float thermo_get_temp (void);

#define	GROUND_SENSOR_OFFSET	0				//offset in volts
#define GROUND_SENSOR_GAIN		1 / 10e-3		//sensor sensitivity in volts / °C
#define GROUND_SENSOR_ADC_GAIN	4.0
#define GROUND_SENSOR_ADC_OFS	0				//adc offset in LSB
#define REF_V					2.500			//reference voltage in volts
#define THERMO_AVERAGING		32				//number of measurments for averaging	

#endif /* THERMO_H_ */
