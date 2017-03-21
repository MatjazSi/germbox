/*
 * pid.h
 *
 * Created: 9.10.2012 17:47:28
 *  Author: Matjaž
 */ 


#ifndef PID_H_
#define PID_H_

#include <stdint.h>


#define TYPE_PID	0
#define TYPE_PI		1
#define TYPE_PD		2
#define TYPE_P		3

#define PID_DEFAULT_MAX_INTEGRALL	500000UL

struct pidStruct
{
	unsigned char type; // type of controller
	float cP; // P-coefficient
	float cI; // I-coefficient
	float cD; // D-coefficient
	float sum; //sum used for integration
	float oldError; // previus error used for derivation
	unsigned int maxIntegrall; // maximum integral
	float upper_limit; // upper limit of pid output
	float lower_limit ;// kower limit of pid output
	float deadband; //deadband
	uint8_t anti_windup; // true if anty windup tehniqe is used
	float old_out;
};

typedef struct pidStruct pidstruct_t;


void pid_init (unsigned char cType, float Pcoeff, float Icoeff, float Dcoeff, struct pidStruct *pid);
void pid_set_P_coefficient (float coeff, struct pidStruct *pid);
void pid_set_I_coefficient (float coeff, struct pidStruct *pid);
void pid_set_D_coefficient (float coeff, struct pidStruct *pid);
void pid_set_max_integral (unsigned long max, struct pidStruct *pid);
void pid_set_limit (float u_lim, float l_lim, struct pidStruct *pid);
void pid_set_deadband (unsigned long band, struct pidStruct *pid);
void pid_reset_int (struct pidStruct *pid);
void pid_anty_windup_enable (struct pidStruct *pid);
float pid_execute (struct pidStruct *pid, float error);



#endif /* PID_H_ */