#include <stdlib.h>

#include "types/result/mug_result_type.h"
#include "types/result/__mug_result.h"
#include "types/result/mug_continuation_result.h"
#include "types/result/__mug_continuation_result.h"
#include "types/result/mug_http_continuation_result.h"
#include "types/result/mug_result_type.h"
#include "types/misc/mug_http_request.h"


struct mug_http_continuation_result {
    mug_continuation_result_t continuation_result;
    mug_http_request_t *http_request;
};


mug_http_continuation_result_t* mug_http_continuation_result_alloc()
{
    return (mug_http_continuation_result_t*)malloc(sizeof(mug_http_continuation_result_t));
}


void mug_http_continuation_result_ctor(mug_http_continuation_result_t *http_continuation_result)
{
    mug_continuation_result_t *continuation_result = (mug_continuation_result_t*)http_continuation_result;
    mug_continuation_result_ctor(continuation_result);

    mug_result_t *result = (mug_result_t*)http_continuation_result;
    result->type = MUG_HTTP_CONTINUATION_RESULT;

    http_continuation_result->http_request = NULL;
}


void mug_http_continuation_result_dtor(mug_http_continuation_result_t *http_continuation_result)
{
    if (http_continuation_result->http_request) {
        mug_http_request_dtor(http_continuation_result->http_request);
        free(http_continuation_result->http_request);
    }
}


mug_http_request_t* mug_http_continuation_result_http_request(const mug_http_continuation_result_t *http_continuation_result)
{
    return http_continuation_result->http_request;
}


void mug_http_continuation_result_set_http_request(mug_http_continuation_result_t *http_continuation_result, mug_http_request_t *http_request)
{
    http_continuation_result->http_request = http_request;
}