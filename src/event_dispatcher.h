#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H


#include "event/event.h"
#include "event/event_type.h"
#include "event_handler/event_handler.h"


struct event_dispatcher;


typedef struct event_dispatcher event_dispatcher_t;


event_dispatcher_t* event_dispatcher_alloc();
void event_dispatcher_ctor(event_dispatcher_t*);
int event_dispatcher_register_handler(event_dispatcher_t*, event_type_t, event_handler_func_t);
int event_dispatcher_raise_event(const event_dispatcher_t*, const event_t*);


#endif