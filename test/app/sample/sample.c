#include <stdio.h>
#include <stdlib.h>

#include "mug/mug.h"


mug_result_t* hello_world_handler(const mug_request_invocation_t *req)
{
    mug_http_response_t *response = mug_http_response_alloc();
    mug_http_response_ctor(response);

    mug_http_response_set_message(response, "OK");

    mug_headers_t *headers = mug_http_response_headers(response);
    mug_headers_append(headers, "Content-Type: application/json");

    const char *payload = "{\n"
                          "    \"Hello\": \"World!\"\n"
                          "}";

    mug_body_t *body = mug_http_response_body(response);
    mug_body_set_content_str(body, payload);


    mug_response_result_t *result = mug_response_result_alloc();
    mug_response_result_ctor(result);

    mug_response_result_set_http_response(result, response);

    return (mug_result_t*)result;
}


int main()
{
    mug_context_t *mug_context = mug_context_alloc();
    mug_context_ctor(mug_context);

    mug_context_add_route_handler(mug_context, "/api/v1/hello", hello_world_handler);

    mug_context_serve(mug_context);

    mug_context_dtor(mug_context);
    free(mug_context);

    return 0;
}