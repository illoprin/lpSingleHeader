/*
	Loprin include-only lib
	#define LP_MATH_IMPLEMENTATION before using
	if you dont want to get undefined reference to ...

	Before using call `srand(time(NULL))` to get more various random values 
*/

#ifndef __LP_MATH_H__
#define __LP_MATH_H__

#include <time.h>
#include <stdlib.h>
#include "lp_common.h"

void lpr_init();
float lpr_clamped_real();
int lpr_integer_uniform(int min, int max);
float lpr_real_uniform(float min, float max);


#ifdef LP_MATH_IMPLEMENTATION

#define ABS_DIFF(a, b) \
	(a) > (b) ? (a) - (b) : (b) - (a)

#define GET_MAX(a, b) \
	(a) > (b) ? (a) : (b)

#define GET_MIN(a, b) \
	(a) < (b) ? (a) : (b)

float lpr_clamped_real()
{
	return (float)rand() / RAND_MAX;
};
int lpr_integer_uniform(int min, int max)
{
	return rand() % ABS_DIFF(min, max) + min;
};
float lpr_real_uniform(float min, float max)
{
	float ratio = lpr_clamped_real();
	return	ABS_DIFF(min, max) * ratio + min;
};

#endif // __LP_MATH_IMPLEMENTATION__


#endif // __LP_MATH_H__