#ifndef EVENT_SOURCE_H
#define EVENT_SOURCE_H


#include "event/event.h"


struct event_source;


typedef struct event_source event_source_t;


event_source_t* event_source_alloc();
void event_source_ctor(event_source_t*, int);
int event_source_get_fd(const event_source_t*);
event_t* event_source_get_event(const event_source_t*);

#endif