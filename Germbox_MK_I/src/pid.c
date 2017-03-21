/*
 * pid.c
 *
 * Created: 9.10.2012 17:47:10
 *  Author: Matjaž
 */ 

/*execution time -O3:
PID:	1210
PI:		1036
PD:		920
P:		616
*/

#include "pid.h"
#include <math.h>
#include <stdlib.h>

void pid_init (unsigned char cType, float Pcoeff, float Icoeff, float Dcoeff, struct pidStruct *pid)
{
	
	pid->type = cType;
	pid->cP = Pcoeff;
	pid->cI = Icoeff;
	pid->cD = Dcoeff;
	pid->oldError = 0;
	pid->sum = 0;
	pid->maxIntegrall = PID_DEFAULT_MAX_INTEGRALL;
	pid->limit = 0;
	pid->deadband = 0;
}

void pid_set_P_coefficient (float coeff, struct pidStruct *pid)
{
	pid->cP = coeff;	
}

void pid_set_I_coefficient (float coeff, struct pidStruct *pid)
{
	pid->cI = coeff;	
}	

void pid_set_D_coefficient (float coeff, struct pidStruct *pid)
{
	pid->cD = coeff;	
}

void pid_set_max_integral (unsigned long max, struct pidStruct *pid)
{
	pid->maxIntegrall = max;
}

void pid_set_limit (unsigned long lim, struct pidStruct *pid)
{
	pid->limit = lim;
}		

void pid_set_deadband (unsigned long band, struct pidStruct *pid)
{
	pid->deadband = band;
}

void pid_reset_int (struct pidStruct *pid)
{
	pid -> sum = 0;
}

float pid_execute (struct pidStruct *pid, float error)
{
	float pTerm = 0, iTerm = 0, dTerm = 0, output = 0;
	if(pid->deadband != 0) // deadband
	{
		if(abs(error) < pid->deadband) {error = 0;}
	}
	
	//calculate p term
	pTerm = pid->cP * error;
	
	//calculate i term if enabled
	if(pid->type == TYPE_PI || pid->type == TYPE_PID)
	{
		pid->sum += error;
		if(pid->sum > (float)pid->maxIntegrall) {pid->sum = pid->maxIntegrall;} //prevents integral windup
		else if(pid->sum < -(float)pid->maxIntegrall) {pid->sum = -pid->maxIntegrall;}
		iTerm = pid->cI * pid->sum;
	}
	
	//calculate d term if enabled
	if(pid->type == TYPE_PD || pid->type == TYPE_PID)
	{
		dTerm = pid->cD * (error - pid->oldError);
		pid->oldError = error;
	}
	output = pTerm + iTerm + dTerm;
	if(pid->limit != 0)
	{
		if(output > pid->limit) {output = pid->limit;}
		else if(output < -pid->limit) {output = -pid->limit;}
	}
	return output;	
}