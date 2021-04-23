#include <stdlib.h>

#include "event/http_request_event.h"
#include "event/internal_event.h"
#include "event/__internal_event.h"
#include "core/handler.h"
#include "types/misc/mug_save_bag.h"
#include "types/misc/mug_http_request.h"
#include "event/event_type.h"


struct http_request_event {
    internal_event_t internal_event;
    mug_continuation_handler_t continuation_handler;
    mug_save_bag_t *save_bag;
    mug_http_request_t *http_request;
};


http_request_event_t* http_request_event_alloc()
{
    return (http_request_event_t*)malloc(sizeof(http_request_event_t));
}


void http_request_event_ctor(
    http_request_event_t *http_request_event,
    mug_continuation_handler_t continuation_handler,
    mug_http_request_t *http_request
    )
{
    internal_event_t *internal_event = (internal_event_t*)http_request_event;
    internal_event_ctor(internal_event);

    http_request_event->internal_event.event.type = HTTP_REQUEST_EVENT;
    http_request_event->continuation_handler = continuation_handler;
    http_request_event->http_request = http_request;


    mug_save_bag_t* save_bag = mug_save_bag_alloc();
    mug_save_bag_ctor(save_bag);
    http_request_event->save_bag = save_bag;
}


void http_request_event_dtor(http_request_event_t *http_request_event)
{
    mug_save_bag_dtor(http_request_event->save_bag);
    free(http_request_event->save_bag);

    mug_http_request_dtor(http_request_event->http_request);
    free(http_request_event->http_request);
}


mug_continuation_handler_t http_request_event_continuation_handler(const http_request_event_t *http_request_event)
{
    return http_request_event->continuation_handler;
}


mug_save_bag_t* http_request_event_save_bag(const http_request_event_t *http_request_event)
{
    return http_request_event->save_bag;
}


mug_http_request_t* http_request_event_http_request(const http_request_event_t *http_request_event)
{
    return http_request_event->http_request;
}