#ifndef EXTERNAL_EVENT_H
#define EXTERNAL_EVENT_H


#include "event/event.h"


typedef struct {
    event_t event;
    int fd;
} external_event_t;


#endif