#include <stdlib.h>

#include "event_handler/event_handler.h"
#include "event/external_event.h"
#include "event/new_connection_event.h"
#include "event_service.h"
#include "routing_table.h"
#include "types/misc/mug_http_request.h"
#include "types/invocation/mug_request_invocation.h"
#include "types/result/mug_response_result.h"
#include "core/handler.h"
#include "http/request_parser.h"
#include "types/invocation/mug_request_invocation.h"
#include "types/result/mug_response_result.h"


static mug_http_request_t* parse_http_request(const new_connection_event_t*);
static mug_request_invocation_t* create_request_invocation(mug_http_request_t*);
static mug_request_handler_t lookup_route_handler(const routing_table_t*, const mug_http_request_t*);
static mug_response_result_t* call_request_handler(mug_request_handler_t, const mug_request_invocation_t*);
static void handle_response_result(const event_service_t *, const new_connection_event_t*, mug_response_result_t*);


void handle_new_connection(void *arg)
{
    struct event_handler_params *params = (struct event_handler_params*)arg;
    const new_connection_event_t *event = (new_connection_event_t*)params->event;  
    const event_service_t *event_service = params->event_service;
    const routing_table_t *routing_table = params->routing_table;

    mug_http_request_t *http_request = parse_http_request(event);
    mug_request_handler_t request_handler = lookup_route_handler(routing_table, http_request);
    mug_request_invocation_t *invocation = create_request_invocation(http_request);
    http_request = NULL;
    mug_response_result_t *response = call_request_handler(request_handler, invocation);
    handle_response_result(event_service, event, response);

    mug_request_invocation_dtor(invocation);
    mug_response_result_dtor(response);
    free(invocation);
    free(response);
}


static mug_http_request_t* parse_http_request(const new_connection_event_t *event)
{
    int sfd = external_event_fd((external_event_t*)event);

    request_parser_t *parser = request_parser_alloc();
    request_parser_ctor(parser, sfd);

    mug_http_request_t *http_request = request_parser_parse(parser);
    return http_request;
}


static mug_request_invocation_t* create_request_invocation(mug_http_request_t *http_request)
{
    mug_request_invocation_t *request_invocation = mug_request_invocation_alloc();
    mug_request_invocation_ctor(request_invocation);
    mug_request_invocation_set_http_request(request_invocation, http_request);

    return request_invocation;
}


static mug_request_handler_t lookup_route_handler(const routing_table_t *routing_table, const mug_http_request_t *http_request)
{
    char url[MAX_URL_LEN];
    mug_http_request_http_url(http_request, url);
    mug_request_handler_t request_handler = routing_table_get_handler(routing_table, url);
    return request_handler;
}


static mug_response_result_t* call_request_handler(mug_request_handler_t request_handler, const mug_request_invocation_t *request_invocation)
{
    mug_response_result_t *response_result = (mug_response_result_t*)request_handler(request_invocation);
    return response_result;
}


static void handle_response_result(const event_service_t *event_service,
                            const new_connection_event_t *event,
                            mug_response_result_t *response_result)
{
    mug_http_response_t *http_response;
    mug_response_result_move_http_response(response_result, &http_response);

    int sfd = external_event_fd((external_event_t*)event);

    event_service_raise_request_completed(event_service, http_response, sfd);
}