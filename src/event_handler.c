#include <unistd.h>
#include <stdio.h>


#include "event_handler.h"
#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "http_parser.h"
#include "routing_table.h"


int handle_request_event(void *arg)
{
    struct event_arg *event_arg = (struct event_arg*)arg;
    io_event_map_t *map = (io_event_map_t*)event_arg->io_event_map;
    io_request_event_t *ev = (io_request_event_t*)event_arg->io_event;
    
    printf("Request Event fd: %d\n", ((io_event_t*)ev)->fd);

    struct mug_request *mug_request = parse_http_request(((io_event_t*)ev)->fd);
    printf("Request Method: %d\n", (int)mug_request->req_method);
    printf("Request URL: %s\n", mug_request->url);

    printf("Headers:\n");
    for (int i = 0; i < mug_request->headers_size; i++) {
        printf("%s\n", mug_request->headers[i]);
    }

    printf("Body:\n");
    printf("%s\n", mug_request->body);

    routing_table_t *routing_table = ev->routing_table;
    route_handler_t handler = routing_table_find_route(routing_table, mug_request->url);

    handler(NULL, NULL);

    mug_request_deinit(mug_request);

    close(((io_event_t*)ev)->fd);

    io_event_map_remove_event(map, (io_event_t*)ev);
    return 0;
}
