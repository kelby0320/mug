#include <stdlib.h>


#include "types/invocation/mug_invocation.h"
#include "types/invocation/__mug_invocation.h"
#include "types/invocation/mug_continuation_invocation.h"
#include "types/invocation/__mug_continuation_invocation.h"
#include "types/invocation/mug_http_continuation_invocation.h"
#include "types/misc/mug_http_response.h"


struct mug_http_continuation_invocation {
    mug_continuation_invocation_t continuation_invocation;
    mug_http_response_t *http_response;
};


mug_http_continuation_invocation_t* mug_http_continuation_invocation_alloc()
{
    return (mug_http_continuation_invocation_t*)malloc(sizeof(mug_continuation_invocation_t));
}


void mug_http_continuation_invocation_ctor(mug_http_continuation_invocation_t *http_continuation_invocation)
{
    mug_continuation_invocation_t *continuation_invocation = (mug_continuation_invocation_t*)http_continuation_invocation;
    mug_continuation_invocation_ctor(continuation_invocation);

    mug_invocation_t *invocation = (mug_invocation_t*)http_continuation_invocation;
    invocation->type = MUG_HTTP_CONTINUATION_INVOCATION;

    http_continuation_invocation->http_response = NULL;
}


void mug_http_continuation_invocation_dtor(mug_http_continuation_invocation_t *http_continuation_invocation)
{
    if (http_continuation_invocation->http_response) {
        mug_http_response_dtor(http_continuation_invocation->http_response);
        free(http_continuation_invocation->http_response);
    }

    mug_continuation_invocation_dtor(&http_continuation_invocation->continuation_invocation);
}


mug_http_response_t* mug_http_continuation_invocation_http_response(const mug_http_continuation_invocation_t *http_continuation_invocation)
{
    return http_continuation_invocation->http_response;
}


void mug_http_continuation_invocation_set_http_response(mug_http_continuation_invocation_t *http_continuation_invocation, mug_http_response_t *http_response)
{
    http_continuation_invocation->http_response = http_response;
}