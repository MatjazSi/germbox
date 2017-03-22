/*
 * thermo.c
 *
 * Created: 20. 03. 2017 00:51:53
 *  Author: matja
 */
#include <math.h>
 
#include "pio.h"
#include "pmc.h"
#include "adc.h"

#include "thermo.h"
#include "hardware_conf.h"

void thermo_init(void)
{
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_ADC);
	pio_set_input(GROUND_TEMP_PORT, GROUND_TEMP_PIN, 0);	
	adc_init(ADC, 36000000, 2000000, 8);
	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
}

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
	//return (((adc * REF_V) / 4096) * GROUND_SENSOR_GAIN);
	//do rounding on one decimal palace
	temp = (((adc * REF_V) / 4096) * GROUND_SENSOR_GAIN);
	whole = roundf(temp);
	decimal = temp - whole;
	temp = decimal * 10;
	whole += (roundf(temp) / 10);
	if(whole - (roundf(whole)) > 0.5)
	{
		whole += 0.1;
	}
		
	return whole;
	
}