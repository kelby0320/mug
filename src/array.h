#ifndef ARRAY_H
#define ARRAY_H


#include <stdlib.h>


struct array {
	void* data;
	size_t elem_size;
	ssize_t size;
};


struct array* array_create(size_t, ssize_t);


void array_destroy(struct array*);


ssize_t array_size(const struct array*);


int array_resize(struct array*, ssize_t);


void* array_get(const struct array*, int);


int array_set(const struct array*, int, const void*);

#endif
