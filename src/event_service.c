#include <stdlib.h>
#include <unistd.h>

#include "event_service.h"
#include "types/misc/mug_http_response.h"
#include "event/request_completed_event.h"
#include "event/event.h"


struct event_service {
    int fd;
};


event_service_t* event_service_alloc()
{
    return (event_service_t*)malloc(sizeof(event_service_t));
}


void event_service_ctor(event_service_t *event_service, int fd)
{
    event_service->fd = fd;
}


int event_service_get_fd(const event_service_t *event_service)
{
    return event_service->fd;
}


void event_service_raise_request_completed(const event_service_t *event_service, mug_http_response_t* http_response, int fd)
{
    request_completed_event_t *request_completed_event = request_completed_event_alloc();
    request_completed_event_ctor(request_completed_event, fd, http_response);

    write(event_service->fd, &request_completed_event, sizeof(request_completed_event_t*));
}
