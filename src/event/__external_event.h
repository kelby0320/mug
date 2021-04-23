#ifndef __EXTERNAL_EVENT_H
#define __EXTERNAL_EVENT_H


#include "event/event.h"
#include "event/__event.h"


struct external_event {
    event_t event;
    int fd;
};


#endif