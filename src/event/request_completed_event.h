#ifndef REQUEST_COMPLETED_EVENT_H
#define REQUEST_COMPLETED_EVENT_H


#include "event/internal_event.h"
#include "types/mug_http_response.h"


typedef struct {
    internal_event_t internal_event;
    int fd;
    struct mug_http_response http_response;
} request_completed_event;


#endif