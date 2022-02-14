#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "event/event.h"
#include "event_source/event_source.h"
#include "event_source/__event_source.h"
#include "event_source/internal_event_source.h"


struct internal_event_source {
    event_source_t event_source;
};


event_t* __get_internal_event_func(const event_source_t* event_source)
{
    // Read event_t object address from event_source socket
    int client_fd = accept(event_source->fd, NULL, NULL);

    event_t *event = 0;
    read(client_fd, &event, sizeof(event_t*));

    close(client_fd);

    return event;
}


internal_event_source_t* internal_event_source_alloc()
{
    return (internal_event_source_t*)malloc(sizeof(internal_event_source_t));
}


void internal_event_source_ctor(internal_event_source_t *internal_event_source, int fd)
{
    event_source_t* event_source = (event_source_t*)internal_event_source;
    event_source_ctor(event_source, fd);

    internal_event_source->event_source.get_event_func = __get_internal_event_func;
}