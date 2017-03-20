/*
 * encoder.c
 *
 * Created: 19. 03. 2017 23:51:47
 *  Author: matja
 */ 
#include "pio.h"
#include "pio_handler.h"
#include "pmc.h"

#include "hardware_conf.h"
#include "encoder.h"

volatile uint8_t enc_oldstate;
volatile int32_t enc_movment;

static void pin_a_handler(void)
{
	if(enc_oldstate == ENC_STATE_C)
	{
		enc_movment--;
	}
	else if(enc_oldstate == ENC_STATE_B) // sanity check
	{
		enc_movment ++;
	}
	enc_oldstate = ENC_STATE_A;
}

static void pin_b_handler(void)
{
	if(enc_oldstate == ENC_STATE_A)
	{
		enc_movment--;
	}
	else if(enc_oldstate == ENC_STATE_C) // sanity check
	{
		enc_movment ++;
	}
	enc_oldstate = ENC_STATE_B;
}

static void pin_c_handler(void)
{
	if(enc_oldstate == ENC_STATE_B)
	{
		enc_movment--;
	}
	else if(enc_oldstate == ENC_STATE_A) // sanity check
	{
		enc_movment ++;
	}
	enc_oldstate = ENC_STATE_C;
}


void encoder_init (void)
{
	volatile uint32_t dummy;
	pmc_enable_periph_clk(ID_PIOA);
	pio_set_input(ENC_PORT, ENC_A_PIN, PIO_PULLUP );
	pio_set_input(ENC_PORT, ENC_B_PIN, PIO_PULLUP );
	pio_set_input(ENC_PORT, ENC_C_PIN, PIO_PULLUP );
	pio_set_input(ENC_PORT, ENC_SW_PIN, PIO_PULLUP);
	
	//get initial state. Function asumes only one pin is low
	if(!pio_get(ENC_PORT, PIO_TYPE_PIO_INPUT, ENC_A_PIN))
	{
		enc_oldstate = ENC_STATE_A;
	}
	else if (!pio_get(ENC_PORT, PIO_TYPE_PIO_INPUT, ENC_B_PIN))
	{
		enc_oldstate = ENC_STATE_B;
	}
	else if (!pio_get(ENC_PORT, PIO_TYPE_PIO_INPUT, ENC_C_PIN))
	{
		enc_oldstate = ENC_STATE_C;
	}
	
	//set interrupts
	pio_handler_set(ENC_PORT, ID_PIOA, ENC_A_PIN, PIO_IT_FALL_EDGE, pin_a_handler);
	pio_handler_set(ENC_PORT, ID_PIOA, ENC_B_PIN, PIO_IT_FALL_EDGE, pin_b_handler);
	pio_handler_set(ENC_PORT, ID_PIOA, ENC_C_PIN, PIO_IT_FALL_EDGE, pin_c_handler);
	pio_enable_interrupt(ENC_PORT, ENC_A_PIN);
	pio_enable_interrupt(ENC_PORT, ENC_B_PIN);
	pio_enable_interrupt(ENC_PORT, ENC_C_PIN);
	NVIC_EnableIRQ(PIOA_IRQn);
	NVIC_ClearPendingIRQ(PIOA_IRQn);
}

void encoder_reset (void) // quick fix for interrupts fiering unexpectedly at the begining
{
	enc_movment = 0;
	if(!pio_get(ENC_PORT, PIO_TYPE_PIO_INPUT, ENC_A_PIN))
	{
		enc_oldstate = ENC_STATE_A;
	}
	else if (!pio_get(ENC_PORT, PIO_TYPE_PIO_INPUT, ENC_B_PIN))
	{
		enc_oldstate = ENC_STATE_B;
	}
	else if (!pio_get(ENC_PORT, PIO_TYPE_PIO_INPUT, ENC_C_PIN))
	{
		enc_oldstate = ENC_STATE_C;
	}
}

int32_t encoder_get (void)
{
	int32_t temp = enc_movment;
	enc_movment = 0;
	return temp;
}