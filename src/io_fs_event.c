#include "io_event.h"
#include "io_fs_event.h"
#include "mug.h"


struct io_fs_event {
    io_event_t event;
    io_event_t *sync_ev;
    struct mug_fs_request *fs_request;
    struct mug_fs_response *fs_response;
};