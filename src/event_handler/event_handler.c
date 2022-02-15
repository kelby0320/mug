#include <stdlib.h>

#include "event_handler/event_handler.h"


void destroy_event_handler_params(struct event_handler_params *params)
{
    if (params->event != NULL) {
        free(params->event);
    }
}