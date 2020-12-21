#include "io_event/io_event.h"
#include "io_event/io_http_event.h"
#include "mug.h"

struct io_http_event {
    io_event_t event;
    io_event_t *sync_ev;
    struct mug_http_request *http_request;
    struct mug_http_response *http_response;
};
