#include <pthread.h>


#include "io_event.h"


struct  io_event_map {
    io_event_t *events;
    pthread_mutex_t mux;
};