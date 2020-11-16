#include "io_event.h"
#include "io_request_event.h"
#include "mug.h"


struct io_request_event {
    io_event_t event;
    io_event_t *sync_ev;
    struct mug_request *request;
    struct mug_response *response;
};