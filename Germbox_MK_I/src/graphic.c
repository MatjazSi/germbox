/*
 * graphic.c
 *
 * Created: 19. 01. 2018 21:09:20
 *  Author: matja
 */ 
#include "ugui/ugui.h"
#include "graphic.h"

void GR_ProgressBar_create (GRprogressbar_t* bar, uint32_t x_pos, uint32_t y_pos, uint32_t width, uint32_t lenght, int32_t minval, int32_t maxval)
{
	bar->x_pos = x_pos;
	bar->y_pos = y_pos;
	bar->width = width;
	bar->lenght = lenght;
	bar->minval = minval;
	bar->maxval = maxval;
	bar->val = minval;
	bar->coef = (lenght * 10) / (maxval - minval); // coefficient of pixels per unit multiplied by 10
}

void GR_ProgressBar_init (GRprogressbar_t *bar)
{
	UG_DrawFrame(bar->x_pos, bar->y_pos, bar->x_pos + bar->lenght, bar->y_pos + bar->width, 1);
	
}
//low2 + (value - low1) * (high2 - low2) / (high1 - low1)
//UG_FillFrame(bar->x_pos + 1, bar->y_pos + 1, bar->x_pos + pixels, (bar->y_pos + 1) + (bar->width - 2), 1); // draw new bar
void GR_ProgressBar_update (GRprogressbar_t *bar, int32_t new_val)
{
	
	uint32_t pixels, mul; // width of new bar
	//pixels = (new_val - bar->minval) * bar->lenght / (bar->maxval - bar->minval); 
	if(new_val < bar->minval)
	{
		new_val = bar->minval;
	}
	if(new_val > bar->maxval)
	{
		new_val = bar->maxval;
	}
	mul = (new_val - bar->minval) * bar->coef;
	pixels = mul / 10;
	if((mul % 10) > 5)
	{
		pixels++;
	}
	
	UG_FillFrame(bar->x_pos + 1, bar->y_pos + 1, bar->lenght - 1, (bar->y_pos + 1) + (bar->width - 2), 0); // delete old bar
	UG_FillFrame(bar->x_pos + 1, bar->y_pos + 1, bar->x_pos + pixels + 1, (bar->y_pos + 1) + (bar->width - 2), 1); // draw new bar
}
 
