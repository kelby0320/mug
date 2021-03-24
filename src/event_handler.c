#include <unistd.h>
#include <stdio.h>

#include "event_handler.h"
#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "http/http_parser.h"
#include "http/http_response_writer.h"
#include "routing_table.h"


int handle_request_event(void *arg)
{
    struct event_arg *event_arg = (struct event_arg*)arg;
    io_event_map_t *map = (io_event_map_t*)event_arg->io_event_map;
    io_request_event_t *ev = (io_request_event_t*)event_arg->io_event;
    int sfd = ((io_event_t*)ev)->fd;

    struct mug_request *mug_request = parse_http_request(sfd);

    routing_table_t *routing_table = ev->routing_table;
    route_handler_t handler = routing_table_find_route(routing_table, mug_request->url);

    ev->request = mug_request;

    struct mug_response_result *result = (struct mug_response_result*)handler(mug_request);

    write_response(sfd, result->result);

    mug_response_deinit(((struct mug_response_result*)result)->result);
    free(result);

    mug_request_deinit(mug_request);

    close(sfd);

    io_event_map_remove_event(map, (io_event_t*)ev);
    return 0;
}
