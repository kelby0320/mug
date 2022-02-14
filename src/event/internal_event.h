#ifndef INTERNAL_EVENT_H
#define INTERNAL_EVENT_H


struct internal_event;


typedef struct internal_event internal_event_t;


internal_event_t* internal_event_alloc();
void internal_event_ctor(internal_event_t*);


#endif