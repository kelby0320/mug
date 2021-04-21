#include <stdlib.h>

#include "types/result/mug_result.h"
#include "types/result/__mug_result.h"
#include "types/result/mug_response_result.h"
#include "types/misc/mug_http_response.h"


struct mug_response_result {
    mug_result_t result;
    mug_http_response_t *http_response;
};


mug_response_result_t* mug_response_result_alloc()
{
    return (mug_response_result_t*)malloc(sizeof(mug_response_result_t));
}


void mug_response_result_ctor(mug_response_result_t *response_result)
{
    mug_result_t *result = (mug_result_t*)response_result;
    result->type = MUG_RESPONSE_RESULT;
    response_result->http_response = NULL;
}


void mug_response_result_dtor(mug_response_result_t *response_result)
{
    if (response_result->http_response) {
        mug_http_response_dtor(response_result->http_response);
        free(response_result->http_response);
    }
}


mug_http_response_t* mug_response_result_http_response(const mug_response_result_t *response_result)
{
    return response_result->http_response;
}


void mug_response_result_set_http_response(mug_response_result_t *response_result, mug_http_response_t *http_response)
{
    response_result->http_response = http_response;
}