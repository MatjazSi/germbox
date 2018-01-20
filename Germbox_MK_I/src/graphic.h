/*
 * graphic.h
 *
 * Created: 19. 01. 2018 21:09:33
 *  Author: matja
 */ 


#ifndef GRAPHIC_H_
#define GRAPHIC_H_

typedef enum
{
	CAROT_NONE,
	CARRIOT_SINGLE,
	CARROT_DOUBLE
}carrot_type_t;

typedef struct 
{
	uint32_t x_pos;
	uint32_t y_pos;
	uint32_t carrot_position; // astores position of ocarrot expresserd in pixels on screen
	uint32_t width;
	uint32_t lenght;
	int32_t minval;			//minimum value of bar
	int32_t maxval;			//maximum value of bar
	int32_t val;
	uint32_t coef;			//coeficient of pixels per unit multiplien by 100
	carrot_type_t carrot_type;
	uint32_t carrot_size;
}GRprogressbar_t;


void GR_ProgressBar_create (GRprogressbar_t* bar, uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t lenght, int32_t minval, int32_t maxval);
void GR_Carrot_create (GRprogressbar_t *bar, carrot_type_t carrot, uint32_t carrot_size );
void GR_Carrot_update (GRprogressbar_t *bar, uint32_t pos);
void GR_ProgressBar_init (GRprogressbar_t *bar);
void GR_ProgressBar_update (GRprogressbar_t *bar, int32_t new_val);


#endif /* GRAPHIC_H_ */