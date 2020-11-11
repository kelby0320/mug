#include "io_event.h"
#include "io_request_event.h"
#include "mug.h"


struct io_request_event {
    io_event_t event;
    void (*handler)(void*, void*);
    mug_request_t *request;
    mug_response_t *response;
};