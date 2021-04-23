#include <stdlib.h>

#include "event/request_completed_event.h"
#include "event/internal_event.h"
#include "event/__internal_event.h"
#include "event/event_type.h"
#include "types/misc/mug_http_response.h"


struct request_completed_event {
    internal_event_t internal_event;
    int fd;
    mug_http_response_t *http_response;
};


request_completed_event_t* request_completed_event_alloc()
{
    return (request_completed_event_t*)malloc(sizeof(request_completed_event_t));
}


void request_completed_event_ctor(
    request_completed_event_t *request_completed_event,
    int fd,
    mug_http_response_t *http_response) 
{
    internal_event_t *internal_event = (internal_event_t*)request_completed_event;
    internal_event_ctor(internal_event);

    request_completed_event->internal_event.event.type = REQUEST_COMPLETED_EVENT;
    request_completed_event->fd = fd;
    request_completed_event->http_response = http_response;
}


void request_completed_event_dtor(request_completed_event_t *request_completed_event)
{
    mug_http_response_dtor(request_completed_event->http_response);
    free(request_completed_event->http_response);
}


int request_completed_event_fd(const request_completed_event_t *request_completed_event)
{
    return request_completed_event->fd;
}


mug_http_response_t* request_completed_event_http_request(const request_completed_event_t *request_completed_event)
{
    return request_completed_event->http_response;
}