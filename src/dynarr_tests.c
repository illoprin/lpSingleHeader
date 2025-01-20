
#define LP_DYNARR_IMPLEMENTATION
#include <lp_dynarr.h>

typedef struct some_struct {
	char name[20];
	unsigned index;
	float val1;
	float val2;
	float val3;
} some_struct_t;

void some_struct_print(some_struct_t* s)
{
	printf("Name: %s; Index: %u; Val1: %.2f; Val2: %.2f; Val3: %.2f\n",
		s->name, s->index, s->val1, s->val2, s->val3);
};

void simple_test(size_t len)
{
	dynarr_t* arr = dynarr_init(int);
	dynarr_reserve(arr, 10);

	
	// Fill array
	for (size_t i = 0; i < len; ++i)
	{
		dynarr_add(arr, (void*)&i);
	};

	printf("Array length is: %lu\n", arr->length);
	printf("Array capacity is: %lu\n", arr->capacity);

	for (size_t i = 0; i < arr->length; ++i)
	{
		int val;
		dynarr_at(arr, (void *)&val, i);
		printf("Index: %lu Value: %d\n", i, val);
	}

	int val_at_5 = dynarr_value_at(arr, int, 5);
	printf("Value at index is 5 - %d\n", val_at_5);

	dynarr_release(arr);
}

void struct_test(size_t len)
{
	dynarr_t* arr = dynarr_init(some_struct_t);

	for (size_t i = 0; i < len; ++i)
	{
		some_struct_t item = {
			.index = 37u,
			.name = "Hello world",
			.val1 = (float)(rand() % 20 - 10),
			.val2 = (float)(rand() % 20 - 10),
			.val3 = (float)(rand() % 20 - 10),
		};
		dynarr_add(arr, (void*)&item);
	};

	some_struct_t new_item = {
		.index = 37u,
		.name = "Shit fuck",
		.val1 = 1337,
		.val2 = 1337,
		.val3 = 1337,
	};
	dynarr_replace(arr, (void *)&new_item, arr->length - 1);

	printf("Struct array length: %lu\n", arr->length);
	printf("Struct array capacity: %lu\n", arr->capacity);

	for (int i = -10; i < 20; ++i)
	{
		some_struct_t ival = {};
		dynarr_at(arr, (void*)&ival, i);
	}

	for (int i = 0; i < arr->length; ++i)
	{
		some_struct_t ival = dynarr_value_at(arr, some_struct_t, i);
	}

	dynarr_release(arr);
}

void replace_test(size_t len)
{
	dynarr_t* arr = dynarr_init(float);

	float new;
	for (size_t i = 0; i < len; ++i)
	{
		new = (float)rand() / RAND_MAX;
		dynarr_add(arr, (void *)&new);
	};

	// Print values
	for (size_t i = 0; i < arr->length; ++i)
		printf("Index: %lu Value: %.2f\n", i, dynarr_value_at(arr, float, i));

	putchar('\n');

	float val;
	for (int i = 0; i < arr->length / 2; ++i)
	{
		val = (float)(i % 4) + 1;
		dynarr_replace(arr, (void *)&val, (size_t)i);
	}


	putchar('\n');

	// Print values
	for (size_t i = 0; i < arr->length; ++i)
		printf("Index: %lu Value: %.2f\n", i, dynarr_value_at(arr, float, i));

	dynarr_release(arr);
}


int main(void)
{
	srand(47384);

	simple_test(10);
	struct_test(20000);
	replace_test(60);

	return 0;
};
