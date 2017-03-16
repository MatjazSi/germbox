/*
 * pump.h
 *
 * Created: 15. 03. 2017 13:37:41
 *  Author: matja
 */ 


#ifndef PUMP_H_
#define PUMP_H_
#include "hardware_conf.h"

void pump_init(void);

#define  pump_on	pio_set_pin_group_high(PUMP_PORT, PUMP_PIN);

#define  pump_off	pio_set_pin_group_low(PUMP_PORT, PUMP_PIN);



#endif /* PUMP_H_ */
