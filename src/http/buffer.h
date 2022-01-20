#ifndef BUFFER_H
#define BUFFER_H


#include <stdlib.h>


struct buffer;


typedef struct buffer buffer_t;


buffer_t* buffer_alloc();
void buffer_ctor(buffer_t*, int);
void buffer_ctor_sized(buffer_t*, int, size_t);
void buffer_dtor(buffer_t*);
char buffer_get_char(buffer_t*);



#endif