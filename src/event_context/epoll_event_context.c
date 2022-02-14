#include <stdlib.h>
#include <sys/epoll.h>

#include "event_context/__event_context.h"
#include "event_context/event_context.h"
#include "event_context/context_event_type.h"
#include "event_context/epoll_event_context.h"


struct epoll_event_context {
    event_context_t event_context;
    int epoll_fd;
};


static int __wait_func(event_context_t*, struct context_event*, int);
static void __add_func(event_context_t*, struct context_event);
static void __remove_func(event_context_t*, struct context_event);


epoll_event_context_t* epoll_event_context_alloc()
{
    return (epoll_event_context_t*)malloc(sizeof(epoll_event_context_t));
}


void epoll_event_context_ctor(epoll_event_context_t *epoll_event_context)
{
    epoll_event_context->event_context.wait_func = __wait_func;
    epoll_event_context->event_context.add_func = __add_func;
    epoll_event_context->event_context.remove_func = __remove_func;
    epoll_event_context->epoll_fd = epoll_create1(0);
}


static int __wait_func(event_context_t *event_context, struct context_event *context_events, int max_events)
{
    epoll_event_context_t *epoll_context = (epoll_event_context_t*)event_context;
    struct epoll_event *ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * max_events);
    
    int nfds = epoll_wait(epoll_context->epoll_fd, ep_events, max_events, -1);

    /* Convert and copy struct epoll_event to struct context_event */
    for (int i = 0; i < nfds; i++) {
        context_events[i].fd = ep_events[i].data.fd;
        
        if (ep_events[i].events == EPOLLIN) {
            context_events[i].type = EVENT_IN;
        }
        else if (ep_events[i].events == EPOLLOUT) {
            context_events[i].type = EVENT_OUT;
        }
    }

    free(ep_events);

    return nfds;
}


static void __add_func(event_context_t *event_context, struct context_event context_event)
{
    epoll_event_context_t *epoll_context = (epoll_event_context_t*)event_context;
    struct epoll_event ev;

    if (context_event.type == EVENT_IN) {
        ev.events = EPOLLIN;
    }
    else if (context_event.type == EVENT_OUT) {
	    ev.events = EPOLLOUT;
    }

    ev.data.fd = context_event.fd;

    epoll_ctl(epoll_context->epoll_fd, EPOLL_CTL_ADD, context_event.fd, &ev);
}


static void __remove_func(event_context_t *event_context, struct context_event context_event)
{
    epoll_event_context_t *epoll_context = (epoll_event_context_t*)event_context;
    struct epoll_event ev;

    ev.data.fd = context_event.fd;
    ev.events = 0;
    epoll_ctl(epoll_context->epoll_fd, EPOLL_CTL_DEL, context_event.fd, &ev);
}


