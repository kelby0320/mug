#include <stdio.h>

#include "mug.h"


static struct mug_result* test_handler(struct mug_request* request);
static struct mug_result* make_result();


int main()
{
    printf("Create mug_ctx\n");
    mug_ctx_t *mug_ctx = mug_ctx_init(8080, 10);

    routing_table_t *tbl = mug_ctx_routing_table(mug_ctx);
    routing_table_add_route(tbl, "/test_route", test_handler);

    printf("Serve\n");
    mug_ctx_serve(mug_ctx);

    return 0;
}


static struct mug_result* test_handler(struct mug_request *request)
{
    printf("Handling Request\n");

    switch (request->req_method) {
        case HTTP_GET:
            printf("Request Type: GET\n");
            break;
        case HTTP_HEAD:
            printf("Request Type: HEAD\n");
            break;
        case HTTP_PUT:
            printf("Request Type: PUT\n");
            break;
        case HTTP_POST:
            printf("Request Type: POST\n");
            break;
        case HTTP_DELETE:
            printf("Request Type: DELETE\n");
            break;
        default:
            printf("Unknown Request Type!\n");
    }

    printf("Request URL: %s\n", request->url);

    for (int i = 0; i < request->headers_size; i++) {
        printf("%s\n", request->headers[0]);
    }

    printf("Request Body:\n%s\n", request->body);

    return make_result();
}


static struct mug_result* make_result()
{
    struct mug_response_result *response_result = (struct mug_response_result*)malloc(sizeof(struct mug_response_result));

    response_result->mug_result.type = RESPONSE_RESULT;

    struct mug_response *response = mug_response_init();
    response->mug_response_base.type = MUG_RESPONSE;
    response->status_code = STATUS_OK;

    response_result->result = response;

    return (struct mug_result*)response_result;
}
