#ifndef EVENT_SERVICE_H
#define EVENT_SERVICE_H


#include "types/misc/mug_http_response.h"


struct event_service;


typedef struct event_service event_service_t;


event_service_t* event_service_alloc();
void event_service_ctor(event_service_t*, const char*);
int event_service_get_fd(const event_service_t*);
void event_service_raise_request_completed(const event_service_t*, mug_http_response_t*, int);


#endif