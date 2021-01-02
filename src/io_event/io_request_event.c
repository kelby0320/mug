#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "mug.h"


struct io_request_event {
    io_event_t event;
    struct mug_request *request;
    struct mug_response *response;
};


static void _deinit_func(io_event_t *io_event)
{
    io_request_event_t *io_req_evt = (io_request_event_t*)io_event;
    struct mug_request *request = io_req_evt->request;
    struct mug_response *response = io_req_evt->response;
    
    /* Deallocate mug_request */
    free(request->url);

    for (int i = 0; i < request->headers_size; i++) {
	free(request->headers[i]);
    }
    free(request->headers);

    free(request->body);

    /* Deallocate mug_response */
    for (int i = 0; i < response->headers_size; i++) {
	free(response->headers[i]);
    }
    free(response->headers);

    free(response->body);

    /* Deallocate io_request_event */
    free(io_req_evt);
}


io_request_event_t* io_request_event_init()
{
    io_request_event_t *io_req_evt = (io_request_event_t*)malloc(sizeof(io_request_event_t));

    io_req_evt->event.deinit_func = _deinit_func;

    return io_req_evt;
}
