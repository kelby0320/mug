#include <stdlib.h>
#include <sys/epoll.h>

#include "event/event.h"
#include "event/event_ctx.h"


int event_ctx_wait(event_ctx_t *ctx, struct event *events, int max_events) {
    return ctx->wait_func(ctx, events, max_events);
}


void event_ctx_add(event_ctx_t *ctx, struct event event) {
    return ctx->add_func(ctx, event);
}
