#include <sys/epoll.h>


#include "event_ctx.h"


struct event_ctx {
    int epoll_fd;
};


event_ctx_t* event_ctx_init()
{
    event_ctx_t *ctx = (event_ctx_t*)malloc(sizeof(event_ctx_t));

    return ctx;
}


void event_ctx_deinit(event_ctx_t *ctx)
{
    return;
}