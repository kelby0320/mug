#include <stdlib.h>

#include "event/internal_event.h"
#include "event/__internal_event.h"
#include "event/event.h"
#include "event/__event.h"
#include "event/event_type.h"


internal_event_t* internal_event_alloc()
{
    return (internal_event_t*)malloc(sizeof(internal_event_t));
}


void internal_event_ctor(internal_event_t *internal_event)
{
    event_t *event = (event_t*)internal_event;
    event_ctor(event);

    internal_event->event.type = INTERNAL_EVENT_BASE;
}