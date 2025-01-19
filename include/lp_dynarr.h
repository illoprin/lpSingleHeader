/*
	Loprin include-only lib
	#define LP_DYNARR_IMPLEMENTATION before using
	if you dont want to get undefined reference to ...
*/

#ifndef __LP_DYNARR_H__
#define __LP_DYNARR_H__

#include "lp_common.h"
#include <stdlib.h>
#include <string.h>

#define DYNARR_INITIAL_CAPACITY 8

typedef struct dynarr {
	void* data;

	size_t capacity;
	size_t length;
	size_t __elem_size;
} dynarr_t;

dynarr_t* __dynarr_init(size_t elem_size);
void dynarr_reserve(dynarr_t* arr, size_t elem_to_reserve);
void dynarr_add(dynarr_t* arr, void* elem);
void dynarr_replace(dynarr_t* arr, void* data, size_t index);
void dynarr_at(dynarr_t* arr, void* dest, size_t index);
void dynarr_release(dynarr_t* arr);

/*
	EXPERIMENTAL:
		Returns a typed value from the data memory location of array.
		If out of range, or another error - returns first element
*/
#define dynarr_value_at(array, type, index) \
	(index) >= 0 && (index) < array->length && sizeof(type) == array->__elem_size ? \
	*( (type*)((char *)array->data + (index) * array->__elem_size) ) : \
	(LP_LOG_ERR("Out of range"), *( (type*)array->data )) \

#define dynarr_init(type) \
	__dynarr_init(sizeof(type)) \

#ifdef LP_DYNARR_IMPLEMENTATION

dynarr_t* __dynarr_init(size_t elem_size)
{
	// Allocate memory for dynarr
	dynarr_t* arr = (dynarr_t*)malloc(sizeof(dynarr_t));
	if (!arr)
	{
		LP_LOG_ERR("Could not allocate memory for array");
		abort();
	}
	arr->length = 0;
	arr->capacity = DYNARR_INITIAL_CAPACITY;
	arr->__elem_size = elem_size;

	// Allocate memory for data
	arr->data = calloc(1, elem_size * arr->capacity);
	if (!arr->data)
	{
		LP_LOG_ERR("Could not allocate memory for data");
		free(arr);
		abort();
	}

	return arr;
};

void dynarr_reserve(dynarr_t* arr, size_t elem_to_reserve)
{
	if (elem_to_reserve > arr->capacity)
	{
		// Allocate memory for some elements
		if (arr->data != NULL)
		{
			arr->capacity = elem_to_reserve;
			arr->data = realloc(arr->data, arr->capacity * arr->__elem_size);
			if (!arr->data)
			{
				LP_LOG_ERR("Could not reserve, allocation error");
				free(arr);
				return;
			}
			#ifdef DYNARR_DEBUG
				LP_LOG_MSG("Reallocate from %lu to %lu elements", arr->capacity, elem_to_reserve);
			#endif
		}
		else
		{
			LP_LOG_ERR("Init array before reserving");
		}
	}
};

void dynarr_add(dynarr_t* arr, void* elem)
{
	if (arr != NULL)
	{
		if (arr->length >= arr->capacity)
		{
			// Allocate some memory for new elements
			arr->capacity *= 2;
			arr->data = realloc(arr->data, arr->capacity * arr->__elem_size);
			if (!arr->data)
			{
				LP_LOG_ERR("Could not push new element, allocation error");
				free(arr);
				return;
			}
			#ifdef LP_DEBUG
				size_t sf = (arr->capacity / 2) * arr->__elem_size;
				size_t ss = arr->capacity * arr->__elem_size;
				LP_LOG_MSG("Reallocate memory from %lu to %lu bytes", sf, ss);
			#endif
		};

		memcpy((char*)arr->data + arr->length * arr->__elem_size, elem, arr->__elem_size);
		
		#ifdef LP_DEBUG
			LP_LOG_MSG("Pushed element to index %lu", arr->length);
		#endif
		arr->length++;
	}
	else
	{
		LP_LOG_ERR("Call dynarr_init() before pushing");
	}
};

void dynarr_replace(dynarr_t* arr, void* data, size_t index)
{
	if (arr && data)
	{
		if (index >= 0 && index < arr->length)
		{
			memcpy((char *)arr->data + index * arr->__elem_size, (char *)data, arr->__elem_size);
		}
		else
		{
			LP_LOG_ERR("Out of range");
		}
	}
	else
	{
		LP_LOG_ERR("Array not initialized");
	}
};

void dynarr_at(dynarr_t* arr, void* dest, size_t index)
{
	if (arr != NULL)
	{
		if (arr->data != NULL)
		{
			// Check indices correction
			if (index >= 0 && index < arr->length)
				memcpy((char *)dest, (char *)arr->data + index * arr->__elem_size, arr->__elem_size);
			else
			{
				memset(dest, 0, arr->__elem_size);
				LP_LOG_ERR("Out of range");
			}
		}
		else
		{
			LP_LOG_ERR("Something went wrong, array is empty");
		}
	}
	else
	{
		LP_LOG_ERR("Init array, before working with it");
	}
};

void dynarr_release(dynarr_t* arr)
{
	if (arr)
	{
		if (arr->data) free(arr->data);

		#ifdef LP_DEBUG
			size_t br = arr->capacity * arr->__elem_size;
			DYNARR_LOG_MSG("Released bytes %lu", br);
		#endif
		free(arr);
	}
};

#endif


#endif // __LP_DYNARR_H__