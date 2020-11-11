#include "io_event.h"
#include "io_http_event.h"
#include "mug.h"

struct io_http_event {
    io_event_t event;
    void (*handler)(void*, void*);
    mug_http_request_t *http_request;
    mug_http_response_t *http_response;
};