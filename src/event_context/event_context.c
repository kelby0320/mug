#include <stdlib.h>

#include "event_context/__event_context.h"
#include "event_context/event_context.h"
#include "event_context/context_event.h"


int event_context_wait(event_context_t *event_context, struct context_event *events, int max_events)
{
    return event_context->wait_func(event_context, events, max_events);
}


void event_context_add(event_context_t *event_context, struct context_event event)
{
    return event_context->add_func(event_context, event);
}


void event_context_remove(event_context_t *event_context, struct context_event event)
{
    return event_context->remove_func(event_context, event);
}    
