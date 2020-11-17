#include <stdlib.h>
#include <pthread.h>


#include "io_event.h"


struct  io_event_map {
    io_event_t *events;
    pthread_mutex_t mtx;
};


io_event_map_t* io_event_map_init()
{
    io_event_map_t* map = (io_event_map_t*)malloc(sizeof(io_event_map_t));

    map->events = NULL;

    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_init(&map->mtx, &mtx_attr);

    pthread_mutexattr_destroy(&mtx_attr);
}


void io_event_deinit(io_event_map_t* map)
{
    /* TODO - free events linked list */

    pthread_mutex_destroy(&map->mtx);


    free(map);
}