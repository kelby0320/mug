#ifndef HTTP_RESPONSE_EVENT_H
#define HTTP_RESPONSE_EVENT_H


#include "core/handler.h"


struct http_response_event;


typedef struct http_response_event http_response_event_t;


http_response_event_t* http_response_event_alloc();
void http_response_event_ctor(http_response_event_t*, mug_continuation_handler_t, int);
void http_response_event_dtor(http_response_event_t*);
mug_continuation_handler_t http_response_event_continuation_handler(const http_response_event_t*);


#endif