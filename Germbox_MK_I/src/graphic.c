/*
 * graphic.c
 *
 * Created: 19. 01. 2018 21:09:20
 *  Author: matja
 */ 
#include "ugui/ugui.h"
#include "graphic.h"

static void draw_triangle (uint32_t x, uint32_t y, uint32_t siz, uint32_t color)
{
	uint32_t n;
	UG_DrawLine(x, y, x, y - siz, color);	
	for(n = 1; n < siz; n++ )
	{
		UG_DrawLine(x - n, y - n, x - n, y - siz, color);
	}
	for(n = 1; n < siz; n++ )
	{
		UG_DrawLine(x + n , y - n, x + n, y - siz, color);
	}
}

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
	bar->carrot_size = 0;
	bar->carrot_type = CAROT_NONE;
}

void GR_Carrot_create (GRprogressbar_t *bar, carrot_type_t carrot, uint32_t carrot_size )
{
	bar->carrot_type = carrot;
	if(carrot != CAROT_NONE)
	{
		bar->carrot_size = carrot_size;
		bar->carrot_position = bar->x_pos + 1;
	}
}

void GR_Carrot_update (GRprogressbar_t *bar, uint32_t pos)
{
	uint32_t pixels, mul; // width of new bar
	//pixels = (new_val - bar->minval) * bar->lenght / (bar->maxval - bar->minval); 
	if(pos < bar->minval)
	{
		pos = bar->minval;
	}
	if(pos > bar->maxval)
	{
		pos = bar->maxval;
	}
	mul = (pos - bar->minval) * bar->coef;
	pixels = mul / 10;
	if((mul % 10) > 5)
	{
		pixels++;
	}
	draw_triangle(bar->carrot_position, bar->y_pos - 1, bar->carrot_size, 0); //delete old one
	draw_triangle(bar->x_pos + pixels, bar->y_pos - 1, bar->carrot_size, 1); //draw new one	
	bar->carrot_position = bar->x_pos + pixels;
}

void GR_ProgressBar_init (GRprogressbar_t *bar)
{
	UG_DrawFrame(bar->x_pos, bar->y_pos, bar->x_pos + bar->lenght, bar->y_pos + bar->width, 1);
	
	if(bar->carrot_type == CARRIOT_SINGLE)
	{
		GR_Carrot_update(bar, bar->minval);
		//draw_triangle(bar->carrot_position, bar->y_pos - 1, bar->carrot_size, 1);	
	}
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
	
	UG_FillFrame(bar->x_pos + 1, bar->y_pos + 1, bar->x_pos + (bar->lenght - 1), (bar->y_pos + 1) + (bar->width - 2), 0); // delete old bar
	UG_FillFrame(bar->x_pos + 1, bar->y_pos + 1, bar->x_pos + pixels + 1, (bar->y_pos + 1) + (bar->width - 2), 1); // draw new bar
}
 
