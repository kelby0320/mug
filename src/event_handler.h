#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H


#include "io_event/io_event.h"
#include "io_event/io_event_map.h"


struct event_arg {
    io_event_map_t *io_event_map;
    io_event_t *io_event;
};


int handle_request_event(void*);
int handle_http_event(void*);
int handle_fs_event(void*);


#endif
