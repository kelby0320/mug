#include <stdlib.h>

#include "event/event.h"
#include "event_source/event_source.h"
#include "event_source/__event_source.h"
#include "event_source/listen_sock_event_source.h"


struct listen_sock_event_source {
    event_source_t event_source;
};


event_t* __get_listen_sock_event_func(event_source_t* event_source)
{
    // TODO
    return NULL;
}


listen_sock_event_source_t* listen_sock_event_source_alloc()
{
    (listen_sock_event_source_t*)malloc(sizeof(listen_sock_event_source_t));
}


void listen_sock_event_source_ctor(listen_sock_event_source_t *listen_sock_event_source, int fd)
{
    event_source_t *event_source = (event_source_t*)listen_sock_event_source;
    event_source_ctor(event_source, fd);

    listen_sock_event_source->event_source.get_event_func = __get_listen_sock_event_func;
}