#include <stdlib.h>


#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "io_event/io_sync_event.h"


struct io_sync_event {
    io_event_t io_event;
    void *data;
    void (*handler)(void*, void*);
    int num_waiting;
    io_request_event_t *parent_req_event;
    io_event_t *child_events;
    size_t child_event_size;
};
