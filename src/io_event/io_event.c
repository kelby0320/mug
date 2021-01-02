#include "io_event/io_event.h"


void io_event_deinit(io_event_t *io_event)
{
    return io_event->deinit_func(io_event);
}
