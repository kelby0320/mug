#ifndef EVENT_CTX_H
#define EVENT_CTX_H


#include "event/event.h"


struct event_ctx;


typedef struct event_ctx event_ctx_t;


typedef int (*wait_func_t)(event_ctx_t*, struct event*, size_t);
typedef void (*add_func_t)(event_ctx_t*, struct event);


struct event_ctx {
    wait_func_t wait_func;
    add_func_t add_func;
};


int event_ctx_wait(event_ctx_t*, struct event*, size_t);


void event_ctx_add(event_ctx_t*, struct event);


#endif
