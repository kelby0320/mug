#ifndef EPOLL_EVENT_CTX_H
#define EPOLL_EVENT_CTX_H


struct epoll_event_context;


typedef struct epoll_event_context epoll_event_context_t;


epoll_event_context_t* epoll_event_context_alloc();
void epoll_event_context_ctor(epoll_event_context_t*);


#endif
