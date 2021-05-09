#ifndef HTTP_RESPONSE_EVENT_SOURCE_H
#define HTTP_RESPONSE_EVENT_SOURCE_H


#include "core/handler.h"


struct http_response_event_source;


typedef struct http_response_event_source http_response_event_source_t;


http_response_event_source_t* http_response_event_source_alloc();
void http_response_event_source_ctor(http_response_event_source_t*, mug_continuation_handler_t, int);


#endif