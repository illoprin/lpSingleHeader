#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define DYNARRAY_INITIAL_CAPACITY 2

#define sdynarr(type)                         \
	struct dynarr_type_t                      \
	{                                         \
		type __dummy;                         \
		type *data;                           \
		size_t length, capacity, __elem_size; \
	}

#define sdynarr_new(array)                                                  \
	do                                                                      \
	{                                                                       \
		array.capacity = DYNARRAY_INITIAL_CAPACITY;                         \
		array.data = calloc(1, sizeof(array.__elem_size) * array.capacity); \
		array.__elem_size = sizeof(array.__dummy);                          \
		array.length = 0;                                                   \
	} while (0);

#define sdynarr_add(array, value)                                                 \
	do                                                                            \
	{                                                                             \
		if (array.length >= array.capacity)                                       \
		{                                                                         \
			array.capacity *= 2;                                                  \
			array.data = realloc(array.data, array.capacity * array.__elem_size); \
		}                                                                         \
		array.data[array.length] = value;                                         \
		++array.length;                                                           \
	} while (0);

#define sdynarr_clear(array)                        \
	do                                              \
	{                                               \
		array.length = 0;                           \
		array.capacity = DYNARRAY_INITIAL_CAPACITY; \
		if (array.data)                             \
		{                                           \
			free(array.data);                       \
		}                                           \
		array.data = calloc(1, array.__elem_size);  \
	} while (0)

#define sdynarr_free(array)   \
	do                        \
	{                         \
		array.capacity = 0;   \
		array.length = 0;     \
		if (array.data)       \
			free(array.data); \
	} while (0)

typedef sdynarr(int) dynarr_int;

int main()
{
	dynarr_int arr;
	sdynarr_new(arr);

	for (int i = 0; i < 20; ++i)
	{
		int val = i * 20;
		sdynarr_add(arr, val);
	}

	for (int i = 0; i < arr.length; ++i)
	{
		printf("Index: %d Value: %d\n", i, arr.data[i]);
	};

	sdynarr_free(arr);

	return 0;
}