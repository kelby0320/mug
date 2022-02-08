#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H


#include <stdlib.h>

#include "event/event.h"
#include "event_service.h"
#include "routing_table.h"


struct event_handler_params {
    const event_t *event;
    const event_service_t *event_service;
    const routing_table_t *routing_table;
};


void destroy_event_handler_params(struct event_handler_params *);
void handle_new_connection(void *arg);
void handle_request_completed(void *arg);


#endif