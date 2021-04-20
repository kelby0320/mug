#include <stdlib.h>
#include <sys/epoll.h>

#include "event/event.h"
#include "event/event_ctx.h"
#include "event/epoll_event_ctx.h"


struct epoll_event_ctx {
    event_ctx_t event_ctx;
    int epoll_fd;
};


static int _wait_func(event_ctx_t *ctx, struct event *events, int max_events)
{
    struct epoll_event_ctx *epoll_ctx = (struct epoll_event_ctx*)ctx;
    struct epoll_event *ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * max_events);
    
    int nfds = epoll_wait(epoll_ctx->epoll_fd, ep_events, max_events, -1);

    /* Convert and copy struct epoll_event to struct event */
    for (int i = 0; i < nfds; i++) {
	events[i].fd = ep_events[i].data.fd;
	
	if (ep_events[i].events == EPOLLIN) {
	    events[i].type = EVENT_IN;
	}
	else if (ep_events[i].events == EPOLLOUT) {
	    events[i].type = EVENT_OUT;
	}
    }

    free(ep_events);

    return nfds;
}


static void _add_func(event_ctx_t *ctx, struct event event)
{
    struct epoll_event_ctx *epoll_ctx = (struct epoll_event_ctx*)ctx;
    struct epoll_event ev;

    if (event.type == EVENT_IN) {
	ev.events = EPOLLIN;
    }
    else if (event.type == EVENT_OUT) {
	ev.events = EPOLLOUT;
    }

    ev.data.fd = event.fd;

    epoll_ctl(epoll_ctx->epoll_fd, EPOLL_CTL_ADD, event.fd, &ev);
}


static void _remove_func(event_ctx_t *ctx, struct event event)
{
    struct epoll_event_ctx *epoll_ctx = (struct epoll_event_ctx*)ctx;
    struct epoll_event ev;

    ev.data.fd = event.fd;
    ev.events = 0;
    epoll_ctl(epoll_ctx->epoll_fd, EPOLL_CTL_DEL, event.fd, &ev);
}


epoll_event_ctx_t* epoll_event_ctx_init()
{
    epoll_event_ctx_t *ctx = (epoll_event_ctx_t*)malloc(sizeof(epoll_event_ctx_t));
    ctx->event_ctx.wait_func = _wait_func;
    ctx->event_ctx.add_func = _add_func;
    ctx->event_ctx.remove_func = _remove_func;
    ctx->epoll_fd = epoll_create1(0);
    return ctx;
}


void epoll_event_ctx_deinit(epoll_event_ctx_t *ctx)
{
    free(ctx);
}
