#ifndef EVENT_SOURCE_MAP_H
#define EVENT_SOURCE_MAP_H


#include "event_source/event_source.h"


struct event_source_map;


typedef struct event_source_map event_source_map_t;


event_source_map_t* event_source_map_alloc();
void event_source_map_ctor(event_source_map_t*);
void event_source_map_ctor2(event_source_map_t*, size_t);
void event_source_map_dtor(event_source_map_t*);
int event_source_map_size(const event_source_map_t*);
int event_source_map_capacity(const event_source_map_t*);
void event_source_map_add_event_source(event_source_map_t*, event_source_t*);
event_source_t* event_source_map_find_event_source(const event_source_map_t*, int);
void event_source_map_remove_event_source(event_source_map_t*, int);


#endif 