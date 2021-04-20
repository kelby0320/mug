#ifndef HTTP_REQUEST_EVENT_H
#define HTTP_REQUEST_EVENT_H


#include "event/internal_event.h"
#include "core/handler.h"
#include "types/mug_save_bag.h"
#include "types/mug_http_request.h"


typedef struct {
    internal_event_t internal_event;
    continuation_handler_t handler;
    mug_save_bag_t *save_bag;
    struct mug_http_request http_request;
} http_request_event_t;


#endif