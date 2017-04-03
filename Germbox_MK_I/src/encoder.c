/**
 * @file encoder.c
 * @author Siworks
 * @date 3 Apr 2017
 * @brief Support for rotary encoder
 *
 * 
 *  Encoder uses external interrupt lines for rotation detection
 *	and polling for pushbutton detection.
 * 
 * 
 */

 
#include "pio.h"
#include "pio_handler.h"
#include "pmc.h"

#include "hardware_conf.h"
#include "encoder.h"

volatile uint8_t enc_oldstate;
volatile int32_t enc_movment;

/**
 * @brief ISR for pin A change interrupt
 */
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

/**
 * @brief ISR for pin B change interrupt
 */
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

/**
 * @brief ISR for pin C change interrupt
 */
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

/**
 * @brief Initiliazation of encoder
 *
 *	This functioon should be caled before usng encoder
 */
void encoder_init (void)
{
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

/**
 * @brief Resets the encoder count
 */
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
/**
 * @brief Returns the number and direction of encoder rotation
 *
 *	Returns the number and direction of rotation since last call of this function.
 *	Positive numbers mean CW rotation, negative means CCW rotation.
 *
 * @return Numer and direction of rotation
 */
int32_t encoder_get (void)
{
	int32_t temp = enc_movment;
	enc_movment = 0;
	return temp;
}

/**
 * @brief Returns the state of encoder pushbutton
 *
 * @return state of encoder pushbutton
 */
uint8_t encoder_get_pb (void)
{
	return pio_get(ENC_PORT, PIO_TYPE_PIO_INPUT, ENC_SW_PIN);
}