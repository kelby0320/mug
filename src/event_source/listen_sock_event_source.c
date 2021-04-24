#include <stdlib.h>
#include <sys/socket.h>

#include "event/event.h"
#include "event_source/event_source.h"
#include "event_source/__event_source.h"
#include "event_source/listen_sock_event_source.h"
#include "event/new_connection_event.h"


struct listen_sock_event_source {
    event_source_t event_source;
};


event_t* __get_listen_sock_event_func(event_source_t* event_source)
{
    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(event_source->fd, (struct sockaddr*)&client_addr, &client_len);

    new_connection_event_t *new_connection_event = new_connection_event_alloc();
    new_connection_event_ctor(new_connection_event, client_fd);

    return new_connection_event;
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