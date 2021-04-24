#ifndef INTERNAL_EVENT_SOURCE_H
#define INTERNAL_EVENT_SOURCE_H


struct internal_event_source;


typedef struct internal_event_source internal_event_source_t;


internal_event_source_t* internal_event_source_alloc();
void internal_event_source_ctor(internal_event_source_t*, int);


#endif