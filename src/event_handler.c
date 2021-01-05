#include <unistd.h>
#include <stdio.h>


#include "event_handler.h"
#include "io_event/io_event.h"
#include "io_event/io_request_event.h"


int handle_request_event(void *arg)
{
    struct event_arg *event_arg = (struct event_arg*)arg;
    io_event_map_t *map = (io_event_map_t*)event_arg->io_event_map;
    io_request_event_t *ev = (io_request_event_t*)event_arg->io_event;
    
    printf("Request Event fd: %d\n", ((io_event_t*)ev)->fd);
    close(((io_event_t*)ev)->fd);

    io_event_map_remove_event(map, (io_event_t*)ev);
    return 0;
}
