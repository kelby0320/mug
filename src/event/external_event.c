#include <stdlib.h>

#include "event/external_event.h"
#include "event/__external_event.h"
#include "event/event_type.h"


external_event_t* external_event_alloc()
{
    return (external_event_t*)malloc(sizeof(external_event_t));
}


void external_event_ctor(external_event_t *external_event, int fd)
{
    event_t *event = (event_t*)external_event;
    event_ctor(event);

    external_event->event.type = EXTERNAL_EVENT_BASE;
    external_event->fd = fd;
}


int external_event_fd(const external_event_t *external_event)
{
    return external_event->fd;
}