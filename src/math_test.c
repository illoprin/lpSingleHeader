
#include <stdio.h>
#define LP_MATH_IMPLEMENTATION
#include <lp_math.h>

int main()
{
	vec2_t a1 = {1, 1.2666};
	vec2_t a2 = {-3, -4};
	vec2_t b1 = {2, 1.444};
	vec2_t b2 = {1.567, 3.44};

	vec2_t i; bool intersects;
	lp_lines_intersection_point(a1, a2, b1, b2, &i, &intersects);

	if (intersects)
		printf("X: %.2f Y: %.2f\n", i.x, i.y);


	a1 = (vec2_t){0.86, 3.05};
	a2 = (vec2_t){0.2, 3.8};
	b1 = (vec2_t){0.11, 2.94};
	b2 = (vec2_t){1.92, 5.8};

	lp_lines_intersection_point(a1, a2, b1, b2, &i, &intersects);

	if (intersects)
		printf("X: %.5f Y: %.5f\n", i.x, i.y);

	return 0;
}