#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>

#include "core/mug_context.h"
#include "event/event_type.h"
#include "event_context/event_context.h"
#include "event_context/epoll_event_context.h"
#include "event_context/context_event.h"
#include "event_context/context_event_type.h"
#include "event_handler/event_handler.h"
#include "event_source/event_source.h"
#include "event_source/listen_sock_event_source.h"
#include "event_source/internal_event_source.h"
#include "event_dispatcher.h"
#include "event_service.h"
#include "event_source_map.h"
#include "routing_table.h"
#include "task_executor/i_task_executor.h"
#include "task_executor/task_executor.h"
#include "task_executor/task_func.h"


#define DEFAULT_PORT 8080
#define MAX_PENDING_CONNECTIONS 1000
#define INTERNAL_EVENT_FILE "/tmp/mug_int.sock"


struct mug_context {
    unsigned short port;
    event_context_t *event_context;
    event_dispatcher_t *event_dispatcher;
    event_source_map_t *event_source_map;
    routing_table_t *routing_table;
};


struct registered_sock_fds {
    int listen_sock_fd;
    int internal_sock_fd;
};

static struct registered_sock_fds register_event_sources(mug_context_t*);
static void construct_event_dispatcher(mug_context_t*, struct registered_sock_fds);
static listen_sock_event_source_t* construct_listen_socket(mug_context_t*);
static internal_event_source_t* construct_internal_socket(mug_context_t*);


mug_context_t* mug_context_alloc()
{
    return (mug_context_t*)malloc(sizeof(mug_context_t));
}


void mug_context_ctor(mug_context_t *mug_context)
{
    mug_context->port = DEFAULT_PORT;

    mug_context->event_context = (event_context_t*)epoll_event_context_alloc();
    epoll_event_context_ctor((epoll_event_context_t*)mug_context->event_context);

    mug_context->event_source_map = event_source_map_alloc();
    event_source_map_ctor(mug_context->event_source_map);

    mug_context->routing_table = routing_table_alloc();
    routing_table_ctor(mug_context->routing_table);

    struct registered_sock_fds sock_fds = register_event_sources(mug_context);
    construct_event_dispatcher(mug_context, sock_fds);
}


void mug_context_dtor(mug_context_t *mug_context)
{
    free(mug_context->event_context);

    free(mug_context->event_dispatcher);

    event_source_map_dtor(mug_context->event_source_map);
    free(mug_context->event_source_map);

    routing_table_dtor(mug_context->routing_table);
    free(mug_context->routing_table);
}


unsigned short mug_context_port(const mug_context_t *mug_context)
{
    return mug_context->port;
}


void mug_context_set_port(mug_context_t *mug_context, unsigned int port)
{
    mug_context->port = port;
}


void mug_context_add_route_handler(mug_context_t *mug_context, const char *route, mug_request_handler_t handler)
{
    routing_table_add_handler(mug_context->routing_table, route, handler);
}


void mug_context_serve(mug_context_t *mug_context)
{
    event_context_t *event_context = mug_context->event_context;
    event_source_map_t *event_source_map = mug_context->event_source_map;
    event_dispatcher_t *event_dispatcher = mug_context->event_dispatcher;

    struct context_event *events = (struct context_event*)malloc(sizeof(struct context_event) * MAX_PENDING_CONNECTIONS);

    for (;;) {
        int nfds = event_context_wait(event_context, events, MAX_PENDING_CONNECTIONS);
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].fd;
            event_source_t *event_source = event_source_map_find_event_source(event_source_map, fd);
            event_t *event = event_source_get_event(event_source);
            event_dispatcher_handle_event(event_dispatcher, event);
        }
    }
}


static void construct_event_dispatcher(mug_context_t *mug_context, struct registered_sock_fds sock_fds)
{
    i_task_executor_t *task_executor = (i_task_executor_t*)task_executor_alloc();
    task_executor_ctor((task_executor_t*)task_executor);

    event_service_t *event_service = event_service_alloc();
    event_service_ctor(event_service, sock_fds.listen_sock_fd);

    mug_context->event_dispatcher = event_dispatcher_alloc();
    event_dispatcher_ctor(mug_context->event_dispatcher, task_executor, event_service);

    event_dispatcher_register_handler(mug_context->event_dispatcher, NEW_CONNECTION_EVENT, handle_new_connection);
    event_dispatcher_register_handler(mug_context->event_dispatcher, REQUEST_COMPLETED_EVENT, handle_request_completed);
}


static struct registered_sock_fds register_event_sources(mug_context_t *mug_context)
{
    listen_sock_event_source_t *listen_sock_src = construct_listen_socket(mug_context);
    internal_event_source_t *internal_sock_src = construct_internal_socket(mug_context);

    struct registered_sock_fds  sock_fds = {
        .listen_sock_fd = event_source_get_fd((event_source_t*)listen_sock_src),
        .internal_sock_fd = event_source_get_fd((event_source_t*)internal_sock_src)
    };

    event_source_map_add_event_source(mug_context->event_source_map, (event_source_t*)listen_sock_src);
    event_source_map_add_event_source(mug_context->event_source_map, (event_source_t*)internal_sock_src);

    struct context_event listen_sock_ctx_evt = {
        .type = EVENT_IN,
        .fd = sock_fds.listen_sock_fd
    };
    event_context_add(mug_context->event_context, listen_sock_ctx_evt);

    struct context_event internal_sock_ctx_evt = {
        .type = EVENT_IN,
        .fd = sock_fds.internal_sock_fd
    };
    event_context_add(mug_context->event_context, internal_sock_ctx_evt);

    return sock_fds;
}


static listen_sock_event_source_t* construct_listen_socket(mug_context_t *mug_context)
{
    char port[8];
    sprintf(port, "%u", mug_context->port);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, port, &hints, &bind_address);

    int listen_sock = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    bind(listen_sock, bind_address->ai_addr, bind_address->ai_addrlen);

    listen(listen_sock, MAX_PENDING_CONNECTIONS);

    listen_sock_event_source_t *listen_sock_src = listen_sock_event_source_alloc();
    listen_sock_event_source_ctor(listen_sock_src, listen_sock);

    return listen_sock_src;
}


static internal_event_source_t* construct_internal_socket(mug_context_t *mug_context)
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, INTERNAL_EVENT_FILE);

    bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
    listen(sfd, MAX_PENDING_CONNECTIONS);

    internal_event_source_t *internal_src = internal_event_source_alloc();
    internal_event_source_ctor(internal_src, sfd);

    return internal_src;
}