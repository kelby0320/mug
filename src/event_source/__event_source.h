#ifndef __EVENT_SOURCE_H
#define __EVENT_SOURCE_H


#include "event/event.h"
#include "event_source/event_source.h"


typedef event_t* (*get_event_func_t)(const event_source_t*);


struct event_source {
    int fd;
    get_event_func_t get_event_func;
};


#endif