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
#include "routing_table.h"
#include "thread_pool.h"
#include  "io_event.h"


#define DEFAULT_MAX_CONN_EV 1000;


struct mug_ctx {
    int port;
    int max_conn_ev;
    event_ctx_t* event_ctx;
    routing_table_t *routing_table;
    thread_pool_t *pool;
    io_event_map_t *event_map;
};


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

    printf("Create listen_sock\n");
    int listen_sock = socket(bind_address->ai_family, bind_address->ai_socktype | SOCK_NONBLOCK, bind_address->ai_protocol);

    printf("Bind listen_sock\n");
    bind(listen_sock, bind_address->ai_addr, bind_address->ai_addrlen);

    printf("Listen\n");
    listen(listen_sock, 10);

    struct event ev;
    ev.type = EVENT_IN;
    ev.fd = listen_sock;

    printf("Add event\n");
    event_ctx_add(mug_ctx->event_ctx, ev);

    int max_events = 10;
    struct event *events = (struct event*)malloc(sizeof(struct event) * max_events);
    
    printf("Event Loop\n");
    for (;;) {
	printf("Wait\n");
	int nfds = event_ctx_wait(mug_ctx->event_ctx, events, max_events);
	for (int i = 0; i < nfds; i++) {
	    if (events[i].fd == listen_sock) {
		printf("listen_sock event\n");
		struct sockaddr_storage client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len);
		close(client_sock);
	    }
	}
    }
}
