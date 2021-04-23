#ifndef HTTP_REQUEST_EVENT_H
#define HTTP_REQUEST_EVENT_H


#include "core/handler.h"
#include "types/misc/mug_http_request.h"
#include "types/misc/mug_save_bag.h"


struct http_request_event;


typedef struct http_request_event http_request_event_t;


http_request_event_t* http_request_event_alloc();
void http_request_event_ctor(http_request_event_t*, mug_continuation_handler_t, mug_http_request_t*);
void http_request_event_dtor(http_request_event_t*);
mug_continuation_handler_t http_request_event_continuation_handler(const http_request_event_t*);
mug_save_bag_t* http_request_event_save_bag(const http_request_event_t*);
mug_http_request_t* http_request_event_http_request(const http_request_event_t*);


#endif