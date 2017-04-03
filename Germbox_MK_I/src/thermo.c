/**
 * @file thermo.c
 * @author Siworks
 * @date 3 Apr 2017
 * @brief Measurment of temperature and moisture
 *
 * 
 */

#include <math.h>
 
#include "pio.h"
#include "pmc.h"
#include "adc.h"

#include "thermo.h"
#include "hardware_conf.h"


/**
 * @brief Initiliazation of measurment system
 *
 *	This functioon should be caled before using measurment system
 */
void thermo_init(void)
{
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_ADC);
	pio_set_input(GROUND_TEMP_PORT, GROUND_TEMP_PIN, 0);
	pio_set_input(GROUND_MOISTURE_PORT, GROUND_MOISTURE_PIN, 0);
	adc_init(ADC, 36000000, 2000000, 8);
	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
	
	//set gain of temp sesnor channels to 4
	adc_enable_anch(ADC);
	adc_set_channel_input_gain(ADC, GROUND_TEMP_ADC_CH, ADC_GAINVALUE_3);
	
	
	
}

/**
 * @brief Returns temperature
 *
 * @return Tempeerature in deg C
 */
float thermo_get_temp (void)
{
	uint32_t adc;
	uint8_t n;
	float whole, decimal, temp;
	adc_enable_channel(ADC, GROUND_TEMP_ADC_CH);
	adc = 0;
	for(n = 0; n < THERMO_AVERAGING; n++)
	{
		adc_start(ADC);
		while(!(ADC->ADC_ISR & ADC_ISR_DRDY)) {}
		adc += adc_get_latest_value(ADC);
	}
	adc /= THERMO_AVERAGING;
	adc += GROUND_SENSOR_ADC_OFS;
	adc_disable_channel(ADC, GROUND_TEMP_ADC_CH);
	return (((adc * REF_V) / (4096 * GROUND_SENSOR_ADC_GAIN)) * GROUND_SENSOR_GAIN);
}

/**
 * @brief Returns moisture of ground
 *
 * @return Moisture of ground on scale from 0 to 10, 10 being maximum
 */
uint16_t thermo_get_moisture (void)
{
	int32_t adc;
	uint8_t n;
	adc_enable_channel(ADC, GROUND_MOISTURE_ADC_CH);
	adc = 0;
	for(n = 0; n < MOISTURE_AVERAGING; n++)
	{
		adc_start(ADC);
		while(!(ADC->ADC_ISR & ADC_ISR_DRDY)) {}
		adc += adc_get_latest_value(ADC);
	}
	adc /= MOISTURE_AVERAGING;
	adc_disable_channel(ADC, GROUND_MOISTURE_ADC_CH);
	adc = (adc + MOISTURE_OFFSET) / MOISTURE_DIV_FACTOR;
	if(adc < 0) 
	{
		adc = 0;	
	}
	return adc;
}