#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>


#include "mug.h"
#include "routing_table.h"
#include "thread_pool.h"
#include  "io_event.h"


#define DEFAULT_MAX_CONN_EV 1000;


struct mug_ctx {
    int port;
    int max_conn_ev;
    int epoll_fd;
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

    mug_ctx->routing_table = routing_table_init();

    mug_ctx->pool = thread_pool_init(0);

    mug_ctx->event_map = io_event_map_init();

    return mug_ctx;
}


void mug_ctx_deinit(mug_ctx_t* mug_ctx)
{
    routing_table_deinit(mug_ctx->routing_table);

    thread_pool_deinit(mug_ctx->pool);

    io_event_map_deinit(mug_ctx->event_map);

    free(mug_ctx);
}