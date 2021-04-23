#include <stdlib.h>

#include "event/event.h"
#include "event_source/event_source.h"
#include "event_source/__event_source.h"
#include "event_source/http_respone_event_source.h"


struct http_response_event_source {
    event_source_t event_source;
};


event_t* __get_http_response_event_func(event_source_t* event_source)
{
    // TODO
    return NULL;
}


http_response_event_source_t* http_response_event_source_alloc()
{
    return (http_response_event_source_t*)malloc(sizeof(http_response_event_source_t));
}


void http_response_event_source_ctor(http_response_event_source_t *http_response_event_source, int fd)
{
    event_source_t *event_source = (event_source_t*)http_response_event_source;
    event_source_ctor(event_source, fd);

    http_response_event_source->event_source.get_event_func = __get_http_response_event_func;
}