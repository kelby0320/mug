#ifndef REQUEST_COMPLETED_EVENT_H
#define REQUEST_COMPLETED_EVENT_H


#include "types/misc/mug_http_response.h"


struct request_completed_event;


typedef struct request_completed_event request_completed_event_t;


request_completed_event_t* request_completed_event_alloc();
void request_completed_event_ctor(request_completed_event_t*, int, mug_http_response_t*);
void request_completed_event_dtor(request_completed_event_t*);
int request_completed_event_fd(const request_completed_event_t*);
mug_http_response_t* request_completed_event_http_response(const request_completed_event_t*);


#endif