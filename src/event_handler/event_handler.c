#include <stdlib.h>

#include "event_handler/event_handler.h"


void destroy_event_handler_params(struct event_handler_params *params)
{
    if (params->event != NULL) {
        free(params->event);
    }
    if (params->event_service != NULL) {
        free(params->event_service);
    }
    if (params->routing_table != NULL) {
        free(params->routing_table);
    }
}