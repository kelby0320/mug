#ifndef MUG_CONTEXT_H
#define MUG_CONTEXT_H


#include "core/handler.h"


struct mug_context;


typedef struct mug_context mug_context_t;


mug_context_t* mug_context_alloc();
void mug_context_ctor(mug_context_t*);
void mug_context_dtor(mug_context_t*);
unsigned short mug_context_port(const mug_context_t*);
void mug_context_set_port(mug_context_t*, unsigned int);
void mug_context_add_route_handler(mug_context_t*, const char*, mug_request_handler_t);
void mug_context_serve(mug_context_t*);


#endif