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

void encoder_init (void);
void encoder_reset (void);
int32_t encoder_get (void);
#endif /* ENCODER_H_ */
