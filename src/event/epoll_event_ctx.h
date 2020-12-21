#ifndef EPOLL_EVENT_CTX_H
#define EPOLL_EVENT_CTX_H


struct epoll_event_ctx;


typedef struct epoll_event_ctx epoll_event_ctx_t;


epoll_event_ctx_t* epoll_event_ctx_init();


void epoll_event_ctx_deinit(epoll_event_ctx_t*);


#endif
