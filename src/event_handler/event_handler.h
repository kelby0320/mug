#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H


#include "event/event.h"
#include "event_service.h"


struct event_handler_params {
    const event_t *event;
    const event_service_t *event_service;
};


#endif