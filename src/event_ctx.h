#ifndef EVENT_CTX_H
#define EVENT_CTX_H


struct event_ctx;


typedef struct event_ctx event_ctx_t;


event_ctx_t* event_ctx_init();


void event_ctx_deinit(event_ctx_t*);


#endif