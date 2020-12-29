#ifndef EVENT_CTX_H
#define EVENT_CTX_H


#include "event/event.h"


struct event_ctx;


typedef struct event_ctx event_ctx_t;


typedef int (*wait_func_t)(event_ctx_t*, struct event*, int);
typedef void (*add_func_t)(event_ctx_t*, struct event);
typedef void (*remove_func_t)(event_ctx_t*, struct event);


struct event_ctx {
    wait_func_t wait_func;
    add_func_t add_func;
    remove_func_t remove_func;
};


int event_ctx_wait(event_ctx_t*, struct event*, int);


void event_ctx_add(event_ctx_t*, struct event);


void event_ctx_remove(event_ctx_t*, struct event);


#endif
