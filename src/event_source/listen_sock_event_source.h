#ifndef LISTEN_SOCK_EVENT_SOURCE_H
#define LISTEN_SOCK_EVENT_SOURCE_H


struct listen_sock_event_source;


typedef struct listen_sock_event_source listen_sock_event_source_t;


listen_sock_event_source_t* listen_sock_event_source_alloc();
void listen_sock_event_source_ctor(listen_sock_event_source_t*);


#endif