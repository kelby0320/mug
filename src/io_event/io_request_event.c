#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "mug.h"
#include "routing_table.h"


static void _deinit_func(io_event_t *io_event)
{
    io_request_event_t *io_req_evt = (io_request_event_t*)io_event;
    struct mug_request *request = io_req_evt->request;
    struct mug_response *response = io_req_evt->response;
    
    mug_request_deinit(request);

    /* Deallocate mug_response */
    for (int i = 0; i < response->headers_size; i++) {
	free(response->headers[i]);
    }
    free(response->headers);

    free(response->body);

    /* Deallocate io_request_event */
    free(io_req_evt);
}


io_request_event_t* io_request_event_init(int fd, routing_table_t *routing_table)
{
    io_request_event_t *io_req_evt = (io_request_event_t*)malloc(sizeof(io_request_event_t));

    io_req_evt->event.deinit_func = _deinit_func;
    io_req_evt->event.type = IO_REQUEST_EVENT;
    io_req_evt->event.fd = fd;
    io_req_evt->routing_table = routing_table;

    return io_req_evt;
}
