/*
 * graphic.h
 *
 * Created: 19. 01. 2018 21:09:33
 *  Author: matja
 */ 


#ifndef GRAPHIC_H_
#define GRAPHIC_H_

typedef struct 
{
	uint32_t x_pos;
	uint32_t y_pos;
	uint32_t width;
	uint32_t lenght;
	int32_t minval;
	int32_t maxval;
	int32_t val;
	uint32_t coef;
}GRprogressbar_t;


void GR_ProgressBar_create (GRprogressbar_t* bar, uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t lenght, int32_t minval, int32_t maxval);
void GR_ProgressBar_init (GRprogressbar_t *bar);
void GR_ProgressBar_update (GRprogressbar_t *bar, int32_t new_val);


#endif /* GRAPHIC_H_ */