#ifndef EVENT_H
#define EVENT_H


#include "event/event_type.h"


struct event;


typedef struct event event_t;


event_t* event_alloc();
void event_ctor(event_t*);
event_type_t event_type(const event_t*);


#endif