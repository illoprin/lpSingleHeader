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
#include <stdbool.h>
#include <math.h>
#include "lp_common.h"

float lpr_clamped_real();
int lpr_integer_uniform(int min, int max);
float lpr_real_uniform(float min, float max);

/*
	Length of 2 component vector
*/
float lp_vec2_len(vec2_t* v);

/*
	Normalization of 2 component vector
	Normalized vector has 1.0 unit length
	and indicate the direction
*/
vec2_t lp_vec2_norm(vec2_t* v);

/*
	Length of 3 component vector
*/
float lp_vec3_len(vec3_t* v);

/*
	Normalization of 3 component vector
	Normalized vector has 1.0 unit length
	and indicate the direction
*/
vec3_t lp_vec3_norm(vec3_t* v);

/*
	a1, a2 - first linedef
	b1, b2 - second linedef
	p* - pointer to intersection point vector
*/
void lp_lines_intersection(
	vec2_t a1, vec2_t a2,
	vec2_t b1, vec2_t b2,
	vec2_t* p, bool* has_intersection
);

/*
	Returns z componenent of two components cross product
	Let's say that symbol '*' = cross product, that means
	lp_cross_2d(v1, v2) = ((v1.x, v1.y, 0) * (v2.x, v2.y, 0)).z
*/
float lp_cross_2d(vec2_t* v1, vec2_t* v2);

/*
	Cross product of two 3 component vectors
*/
vec3_t lp_vec3_cross(vec3_t* v1, vec3_t* v2);

/*
	Dot product of two 3 component vectors
*/
float lp_vec3_dot(vec3_t* v1, vec3_t* v2);

typedef union lp_vec4 {
	struct {
		float x, y, z, w;
	};
	struct {
		float r, g, b, a;
	};
	float val[4];
} vec4_t;

typedef union lp_vec3 {
	struct {
		float x, y, z;
	};
	struct {
		float r, g, b;
	};
	float val[3];
} vec3_t;

typedef union lp_vec2 {
	struct {
		float x, y;
	};
	struct {
		float u, v;
	};
	struct {
		float s, t;
	};
	float val[2];
} vec2_t;

#define ABS_DIFF(a, b) \
	(a) > (b) ? (a) - (b) : (b) - (a)

#define GET_MAX(a, b) \
	(a) > (b) ? (a) : (b)

#define GET_MIN(a, b) \
	(a) < (b) ? (a) : (b)

#define VEC3_TO_ARR(vec) \
	{vec.x, vec.y, vec.z}

#define VEC2_TO_ARR(vec) \
	{vec.x, vec.y}

#ifdef LP_MATH_IMPLEMENTATION /* Implementation starts here ... */

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

float lp_vec2_len(vec2_t* v)
{
	return sqrtf(v->x * v->x + v->y * v->y);
};

vec2_t lp_vec2_norm(vec2_t* v)
{
	float len = lp_vec2_len(v);
	return (vec2_t){v->x / len, v->y / len};
};

float lp_vec3_len(vec3_t* v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
};

vec3_t lp_vec3_norm(vec3_t* v)
{
	float len = lp_vec3_len(v);
	return (vec3_t){v->x / len, v->y / len, v->z / len};
};

float lp_cross_2d(vec2_t* v1, vec2_t* v2)
{
	return v1->x * v2->y - v1->y * v2->x;
};


vec3_t lp_vec3_cross(vec3_t* v1, vec3_t* v2)
{
	return (vec3_t){
		v1->y * v2->z - v1->z * v2->y,
		v1->z * v2->x - v1->x * v2->z,
		v1->x * v2->y - v1->y * v2->x,
	};
};

float lp_vec3_dot(vec3_t* v1, vec3_t* v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
};

void lp_lines_intersection(
	vec2_t a1, vec2_t a2,
	vec2_t b1, vec2_t b2,
	vec2_t* p, bool* has_intersection
)
{
	// First line direction (a1 + q) = a2
	vec2_t q = {a2.x - a1.x, a2.y - a1.y};

	// Second line direction (b1 + s) = b2
	vec2_t s = {b2.x - b1.x, b2.y - b1.y};

	
	// Linedef between first points of two lines
	vec2_t a1b1 = {b1.x - a1.x, b1.y - a1.y};

	// Intersection point is 'i' ->
	// i = a1 + q * v = b1 + s * t
	// Solving a simple equation we has ->
	// t = (a1 * q - b1 * q) / s * q , where '*' is cross product
	float numerator = lp_cross_2d(&a1b1, &q);
	float denumerator = lp_cross_2d(&q, &s);

	if (denumerator == 0.f)
	{
		// Linedefs are parallel
		*has_intersection = false;
		return;
	}

	float t = numerator / denumerator;

	if (numerator == 0.f && denumerator == 0.f)
	{
		// Linedefs are collinear
		*has_intersection = false;
		return;
	}

	if (t >= 0.f && t <= 1.f)
	{
		// Has intersection point
		p->x = b1.x + s.x * t;
		p->y = b1.y + s.y * t;
		*has_intersection = true;
	}
};

#endif /* Implementation ends here ... */


#endif // __LP_MATH_H__