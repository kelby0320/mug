#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "mug.h"


struct io_request_event {
    io_event_t event;
    struct mug_request *request;
    struct mug_response *response;
};
