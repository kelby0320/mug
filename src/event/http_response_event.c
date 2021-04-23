#include "stdlib.h"

#include "event/http_response_event.h"
#include "event/external_event.h"
#include "event/__external_event.h"
#include "event/event_type.h"
#include "core/handler.h"
#include "types/misc/mug_save_bag.h"


struct http_response_event {
    external_event_t external_event;
    mug_continuation_handler_t continuation_handler;
    mug_save_bag_t *save_bag;
};


http_response_event_t* http_response_event_alloc()
{
    return (http_response_event_t*)malloc(sizeof(http_response_event_t));
}


void http_response_event_ctor(
    http_response_event_t *http_response_event,
    mug_continuation_handler_t continuation_handler,
    int fd)
{
    external_event_t *external_event = (external_event_t*)http_response_event;
    external_event_ctor(external_event, fd);

    http_response_event->external_event.event.type = HTTP_RESPONSE_EVENT;
    http_response_event->continuation_handler = continuation_handler;
    
    mug_save_bag_t *save_bag = mug_save_bag_alloc();
    mug_save_bag_ctor(save_bag);
    http_response_event->save_bag = save_bag;
}


mug_continuation_handler_t http_response_event_continuation_handler(const http_response_event_t *http_response_event)
{
    return http_response_event->continuation_handler;
}