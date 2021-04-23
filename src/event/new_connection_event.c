#include <stdlib.h>

#include "event/new_connection_event.h"
#include "event/external_event.h"
#include "event/__external_event.h"
#include "event/event_type.h"


struct new_connection_event {
    external_event_t external_event;
};


new_connection_event_t* new_connection_event_alloc()
{
    return (new_connection_event_t*)malloc(sizeof(new_connection_event_t));
}


void new_connnection_event_ctor(new_connection_event_t *new_connection_event, int fd)
{
    external_event_t *external_event = (external_event_t*)new_connection_event;
    external_event_ctor(external_event, fd);

    new_connection_event->external_event.event.type = NEW_CONNECTION_EVENT;
}