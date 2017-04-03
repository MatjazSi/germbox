/*
 * encoder.h
 *
 * Created: 19. 03. 2017 23:52:02
 *  Author: matja
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#define ENC_STATE_A		0
#define ENC_STATE_B		1
#define ENC_STATE_C		2

void encoder_init (void); /* Initializes encoder */
void encoder_reset (void); /* Resets encoder count */
int32_t encoder_get (void); /* Returns number and direction of turn of encoder */
uint8_t encoder_get_pb (void); /* Returns the state of pusshbuton of encoder */
#endif /* ENCODER_H_ */
