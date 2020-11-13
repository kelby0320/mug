#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>


#include "mug.h"
#include "routing_table.h"
#include "thread_pool.h"
#include  "io_event.h"


#define DEFAULT_MAX_CONN_EV 1000;
#define DEFAULT_AUX_CONN_MULTIPLE 10;


struct mug_ctx {
    int port;
    routing_table_t *routing_table;
    struct epoll_event *events;
    size_t events_size;
    int max_conn_ev;
    int max_aux_ev;
    thread_pool_t *pool;
    io_event_map_t *event_map;
};


mug_ctx_t* mug_ctx_init(int port, int max_conn)
{
    mug_ctx_t *mug_ctx = (mug_ctx_t*)malloc(sizeof(mug_ctx_t));

    mug_ctx->port = port;

    mug_ctx->routing_table = routing_table_init();

    mug_ctx->pool = thread_pool_init(0);

    if (max_conn <= 0) {
        max_conn = DEFAULT_MAX_CONN_EV;
    }

    int max_aux = thread_pool_size(mug_ctx) * DEFAULT_AUX_CONN_MULTIPLE;
    int max_ev = max_conn + max_aux;

    mug_ctx->max_conn_ev = max_conn;
    mug_ctx->max_aux_ev = max_aux; 
    mug_ctx->events_size = max_ev;

    mug_ctx->events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * mug_ctx->events_size);

    mug_ctx->event_map = io_event_map_init();

    return mug_ctx;
}


