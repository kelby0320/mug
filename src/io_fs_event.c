#include "io_event.h"
#include "io_fs_event.h"
#include "mug.h"


struct io_fs_event {
    io_event_t event;
    void (*handler)(void*, void*);
    mug_fs_request_t *fs_request;
    mug_fs_response_t *fs_response;
};