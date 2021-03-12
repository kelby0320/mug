#ifndef IO_REQUEST_EVENT_H
#define IO_REQUEST_EVENT_H

#include "routing_table.h"


typedef struct {
    io_event_t event;
    struct mug_request *request;
    struct mug_response *response;
    routing_table_t *routing_table;
} io_request_event_t;


io_request_event_t* io_request_event_init(int, routing_table_t*);


#endif
