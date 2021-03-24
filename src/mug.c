#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

#include "mug.h"
#include "event/event.h"
#include "event/event_ctx.h"
#include "event/epoll_event_ctx.h"
#include "event_handler.h"
#include "routing_table.h"
#include "thread_pool.h"
#include  "io_event/io_event_map.h"


#define DEFAULT_MAX_CONN_EV 1000;


struct mug_ctx {
    int port;
    int max_conn_ev;
    event_ctx_t* event_ctx;
    routing_table_t *routing_table;
    thread_pool_t *pool;
    io_event_map_t *event_map;
};


static void delagate_to_thread_pool(mug_ctx_t*, struct event);


struct mug_request* mug_request_init()
{
    struct mug_request *mug_request = (struct mug_request*)calloc(1, sizeof(struct mug_request));
    return mug_request;
}


void mug_request_deinit(struct mug_request *mug_request)
{
    if (mug_request->headers != NULL) {
        for (int i = 0; i < mug_request->headers_size; i++) {
            free(mug_request->headers[i]);
        }
    }

    if (mug_request->body != NULL) {
        free(mug_request->body);
    }

    free(mug_request);
}


struct mug_response* mug_response_init()
{
    struct mug_response *response = (struct mug_response*)calloc(1, sizeof(struct mug_response));
    return response;
}


void mug_response_deinit(struct mug_response *response)
{
    if (response->headers != NULL) {
        for (int i = 0; i < response->headers_size; i++) {
            free(response->headers[i]);
        }
    }

    if (response->body != NULL) {
        free(response->body);
    }

    free(response);
}


mug_ctx_t* mug_ctx_init(int port, int max_conn)
{
    mug_ctx_t *mug_ctx = (mug_ctx_t*)malloc(sizeof(mug_ctx_t));

    mug_ctx->port = port;

    if (max_conn <= 0) {
        max_conn = DEFAULT_MAX_CONN_EV;
    }

    mug_ctx->max_conn_ev = max_conn;

    mug_ctx->event_ctx = (event_ctx_t*)epoll_event_ctx_init();

    mug_ctx->routing_table = routing_table_init();

    mug_ctx->pool = thread_pool_init(0);

    mug_ctx->event_map = io_event_map_init();

    return mug_ctx;
}


void mug_ctx_deinit(mug_ctx_t *mug_ctx)
{
    epoll_event_ctx_deinit((epoll_event_ctx_t*)mug_ctx->event_ctx);

    routing_table_deinit(mug_ctx->routing_table);

    thread_pool_deinit(mug_ctx->pool);

    io_event_map_deinit(mug_ctx->event_map);

    free(mug_ctx);
}


void mug_ctx_serve(mug_ctx_t *mug_ctx)
{
    /*
     * 1) Establish listening socket
     * 2) Begin event loop
     * 3) Pass events to thread_pool workers
     */

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    int listen_sock = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    bind(listen_sock, bind_address->ai_addr, bind_address->ai_addrlen);

    listen(listen_sock, 10);

    struct event ev;
    ev.type = EVENT_IN;
    ev.fd = listen_sock;

    event_ctx_add(mug_ctx->event_ctx, ev);

    int max_events = 10;
    struct event *events = (struct event*)malloc(sizeof(struct event) * max_events);
    
    for (;;) {
        int nfds = event_ctx_wait(mug_ctx->event_ctx, events, max_events);
        for (int i = 0; i < nfds; i++) {
            if (events[i].fd == listen_sock) {
                struct sockaddr_storage client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len);

                ev.type = EVENT_IN;
                ev.fd = client_fd;
                event_ctx_add(mug_ctx->event_ctx, ev);
            } else {
                event_ctx_remove(mug_ctx->event_ctx, ev);
                delagate_to_thread_pool(mug_ctx, events[i]);
            }
        }
    }
}


routing_table_t* mug_ctx_routing_table(mug_ctx_t *mug_ctx)
{
    return mug_ctx->routing_table;
}


static void delagate_to_thread_pool(mug_ctx_t *mug_ctx, struct event event)
{
    printf("delagate to thread pool\n");

    thread_pool_t *tpool = mug_ctx->pool;
    io_event_map_t *event_map = mug_ctx->event_map;
    routing_table_t *routing_table = mug_ctx->routing_table;
    io_event_t *io_event = io_event_map_find(event_map, event.fd);

    if (io_event == NULL) {
        /* Event is a new connection */
        printf("Event is a new connection\n");

        /* Create io_event and add it to the event map */
        io_request_event_t *io_req_evt = io_request_event_init(event.fd, routing_table);
        io_event_map_add_req_event(event_map, io_req_evt);

        /* Schedule event hanlder for this event */
        struct event_arg *arg = (struct event_arg*)malloc(sizeof(struct event_arg));
        arg->io_event_map = event_map;
        arg->io_event = (io_event_t*)io_req_evt;

        printf("Submitting event to thread pool\n");
        thread_pool_submit(tpool, handle_request_event, arg);	
    } else {
	    printf("!!! Event is not a new connection !!!\n");
    }
}


