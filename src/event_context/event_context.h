#ifndef EVENT_CONTEXT_H
#define EVENT_CONTEXT_H


#include "event_context/context_event.h"


struct event_context;


typedef struct event_context event_context_t;


int event_context_wait(event_context_t*, struct context_event*, int);
void event_context_add(event_context_t*, struct context_event);
void event_context_remove(event_context_t*, struct context_event);


#endif
