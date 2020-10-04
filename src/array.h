#ifndef ARRAY_H
#define ARRAY_H


#include <stdlib.h>


typedef struct {
	void* data;
	size_t elem_size;
	ssize_t size;
} array_t;


array_t* array_create(size_t, ssize_t);


void array_destroy(array_t*);


ssize_t array_size(const array_t*);


int array_resize(array_t*, ssize_t);


void* array_get(const array_t*, int);


int array_set(const array_t*, int, const void*);

#endif
