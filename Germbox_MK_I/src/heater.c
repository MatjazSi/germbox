/*
 * heater.c
 *
 * Created: 15. 03. 2017 02:12:48
 *  Author: matja
 */ 


#include "pwm.h"
#include "pio.h"
#include "pmc.h"
#include <stdint.h>

pwm_channel_t pwm_ch_instance;

void heater_init(void)
{
	
	pwm_clock_t pwm_clk;
	
	pmc_enable_periph_clk(ID_PIOA);
	pio_set_peripheral(PIOA, PIO_TYPE_PIO_PERIPH_A, PIO_PA2);
	
	pmc_enable_periph_clk(ID_PWM);
	pwm_channel_disable(PWM, PWM_CHANNEL_2);
	pwm_clk.ul_clka = 50 * 100;
	pwm_clk.ul_clkb = 0;
	pwm_clk.ul_mck = 32000000;
	pwm_init(PWM, &pwm_clk);
	
	
	
	pwm_ch_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
	pwm_ch_instance.ul_period = 100;
	pwm_ch_instance.ul_duty = 3;
	pwm_ch_instance.channel = PWM_CHANNEL_2;
	pwm_ch_instance.alignment = PWM_ALIGN_LEFT;
	pwm_ch_instance.polarity = PWM_HIGH;
	pwm_channel_init(PWM, &pwm_ch_instance);
	pwm_channel_enable(PWM, PWM_CHANNEL_2);
	
}

volatile void heater_set (uint8_t heat)
{
	if(heat > 100)
	{
		heat = 100;
	}
	pwm_channel_update_duty(PWM, &pwm_ch_instance, (uint32_t)heat);
}