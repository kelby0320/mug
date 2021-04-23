#include <stdlib.h>

#include "event/event.h"
#include "event_source/event_source.h"
#include "event_source/__event_source.h"
#include "event_source/internal_event_source.h"


struct internal_event_source {
    event_source_t event_source;
};


event_t* __get_internal_event_func(event_source_t* event_source)
{
    // TODO
    return NULL;
}


internal_event_source_t* internal_event_source_alloc()
{
    (internal_event_source_t*)malloc(sizeof(internal_event_source_t));
}


void internal_event_source_ctor(internal_event_source_t *internal_event_source, int fd)
{
    event_source_t* event_source = (event_source_t*)internal_event_source;
    event_source_ctor(event_source, fd);

    internal_event_source->event_source.get_event_func = __get_internal_event_func;
}