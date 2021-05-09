#include <stdlib.h>

#include "event/event.h"
#include "event_source/event_source.h"
#include "event_source/__event_source.h"
#include "event_source/http_response_event_source.h"
#include "core/handler.h"
#include "event/http_response_event.h"


struct http_response_event_source {
    event_source_t event_source;
    mug_continuation_handler_t continuation_handler;
};


event_t* __get_http_response_event_func(event_source_t* event_source)
{
    http_response_event_source_t *http_response_event_source = (http_response_event_source_t*)event_source;

    mug_continuation_handler_t continuation_handler = http_response_event_source->continuation_handler;
    int fd = event_source->fd;

    http_response_event_t *http_response_event = http_response_event_alloc();
    http_response_event_ctor(http_response_event, continuation_handler, fd);

    return (event_t*)http_response_event;
}


http_response_event_source_t* http_response_event_source_alloc()
{
    return (http_response_event_source_t*)malloc(sizeof(http_response_event_source_t));
}


void http_response_event_source_ctor(
    http_response_event_source_t *http_response_event_source,
    mug_continuation_handler_t continuation_handler,
    int fd)
{
    event_source_t *event_source = (event_source_t*)http_response_event_source;
    event_source_ctor(event_source, fd);

    http_response_event_source->event_source.get_event_func = __get_http_response_event_func;
    http_response_event_source->continuation_handler = continuation_handler;
}