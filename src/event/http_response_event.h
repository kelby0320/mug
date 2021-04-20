#ifndef HTTP_RESPONSE_EVENT_H
#define HTTP_RESPONSE_EVENT_H


#include "event/external_event.h"
#include "core/handler.h"
#include "types/mug_save_bag.h"


typedef struct {
    external_event_t external_event;
    continuation_handler_t handler;
    mug_save_bag_t *save_bag;
} http_response_event_t;


#endif