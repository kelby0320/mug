#ifndef __EVENT_CONTEXT_H
#define __EVENT_CONTEXT_H


#include "event_context/context_event.h"
#include "event_context/event_context.h"


typedef int (*wait_func_t)(event_context_t*, struct context_event*, int);
typedef void (*add_func_t)(event_context_t*, struct context_event);
typedef void (*remove_func_t)(event_context_t*, struct context_event);


struct event_context {
    wait_func_t wait_func;
    add_func_t add_func;
    remove_func_t remove_func;
};


#endif