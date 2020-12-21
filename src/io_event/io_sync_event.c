#include "io_event/io_event.h"
#include "io_event/io_sync_event.h"


struct io_sync_event {
    io_event_t io_event;
    void (*handler)(void*, void*);
    int num_waiting;
};
