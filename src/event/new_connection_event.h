#ifndef NEW_CONNECTION_EVENT_H
#define NEW_CONNECTION_EVENT_H


struct new_connection_event;


typedef struct new_connection_event new_connection_event_t;


new_connection_event_t* new_connection_event_alloc();
void new_connection_event_ctor(new_connection_event_t*, int);



#endif