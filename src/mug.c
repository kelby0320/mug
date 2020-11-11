#include <sys/epoll.h>


#include "mug.h"
#include "route.h"
#include "thread_pool.h"
#include  "io_event.h"


struct mug_ctx {
    char *ip_addr;
    int port;
    route_item_t** routes;
    struct epoll_event *events;
    thread_pool_t *pool;
    io_event_map_t *event_map;
};