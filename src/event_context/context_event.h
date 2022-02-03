#ifndef CONTEXT_EVENT_H
#define CONTEXT_EVENT_H


#include "event_context/context_event_type.h"


struct context_event {
    context_event_type_t type;
    int fd;
};


#endif
