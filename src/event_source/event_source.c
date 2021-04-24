#include <stdlib.h>

#include "event_source/event_source.h"
#include "event_source/__event_source.h"


event_t* __get_event_func(event_source_t* event_source)
{
    return NULL;
}


event_source_t* event_source_alloc()
{
    return (event_source_t*)malloc(sizeof(event_source_t));
}


void event_source_ctor(event_source_t *event_source)
{
    event_source->fd = -1;
    event_source->get_event_func = __get_event_func;
}


int event_source_get_fd(const event_source_t *event_source)
{
    return event_source->fd;
}


event_t* event_source_get_event(const event_source_t *event_source)
{
    return event_source->get_event_func(event_source);
}

