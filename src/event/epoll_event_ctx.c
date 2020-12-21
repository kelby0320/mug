#include <stdlib.h>
#include <sys/epoll.h>

#include "event/event.h"
#include "event/event_ctx.h"
#include "event/epoll_event_ctx.h"


struct epoll_event_ctx {
    event_ctx_t event_ctx;
    int epoll_fd;
};


static int _wait_func(event_ctx_t *ctx, struct event *events, size_t max_events) {
    return 0;
};


static void _add_func(event_ctx_t *ctx, struct event event) {
    return;
};


epoll_event_ctx_t* epoll_event_ctx_init() {
    epoll_event_ctx_t *ctx = (epoll_event_ctx_t*)malloc(sizeof(epoll_event_ctx_t));
    ctx->event_ctx.wait_func = _wait_func;
    ctx->event_ctx.add_func = _add_func;
}


void epoll_event_ctx_deinit(epoll_event_ctx_t *ctx) {
    free(ctx);
}
