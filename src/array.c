#include <stdlib.h>
#include <string.h>


#include "array.h"


array_t* array_create(size_t elem_size, ssize_t size)
{
	array_t* array = (array_t*)malloc(sizeof(array_t));
	array->data = calloc(size, elem_size);
	array->elem_size = elem_size;
	array->size = size;
	return array;
}


void array_destroy(array_t* array)
{
	free(array->data);
	free(array);
}


ssize_t array_size(const array_t* array)
{
	return array->size;
}


int array_resize(array_t* array, ssize_t new_size)
{
	void *new_data = calloc(new_size, array->elem_size);
	void *old_data = array->data;

	/* Only copy minimum of array->size and new_size */
	int copy_amt = array->size < new_size ? array->size : new_size;

	memcpy(new_data, old_data, copy_amt * array->elem_size);

	array->data = new_data;
	array->size = new_size;
	free(old_data);
	return array->size;
}


void* array_get(const array_t* array, int i)
{
	if (i >= array->size)
		return NULL;

	return array->data + (i * array->elem_size);
}


int array_set(const array_t* array, int i, const void* val)
{
	if (i >= array->size)
		return -i;

	void *elem = array->data + (i * array->elem_size);
	memcpy(elem, val, array->elem_size);
	return i;
}
