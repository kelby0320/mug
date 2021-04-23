#include <stdlib.h>

#include "event/event.h"
#include "event/__event.h"


event_t* event_alloc()
{
    return (event_t*)malloc(sizeof(event_t));
}


void event_ctor(event_t *event)
{
    event->type = EVENT_BASE;
}


event_type_t event_type(const event_t *event)
{
    return event->type;
}