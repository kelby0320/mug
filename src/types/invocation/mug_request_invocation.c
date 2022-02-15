#include <stdlib.h>
#include <string.h>

#include "types/invocation/mug_request_invocation.h"
#include "types/invocation/mug_invocation.h"
#include "types/invocation/__mug_invocation.h"
#include "types/misc/mug_http_request.h"
#include "types/misc/mug_http_method.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"


#define MAX_URI_LEN 128


struct mug_request_invocation {
    mug_invocation_t invocation;
    mug_http_request_t *http_request;
};


mug_request_invocation_t* mug_request_invocation_alloc()
{
    return (mug_request_invocation_t*)malloc(sizeof(mug_request_invocation_t));
}


void mug_request_invocation_ctor(mug_request_invocation_t *request_invocation)
{
    mug_invocation_t *invocation = (mug_invocation_t*)request_invocation;
    mug_invocation_ctor(invocation);

    request_invocation->invocation.type = MUG_REQUEST_INVOCATION;
    request_invocation->http_request = NULL;
}


void mug_request_invocation_dtor(mug_request_invocation_t *request_invocation)
{
    if (request_invocation->http_request) {
        mug_http_request_dtor(request_invocation->http_request);
        free(request_invocation->http_request);
    }
}


mug_http_request_t* mug_request_invocation_http_request(const mug_request_invocation_t *request_invocation)
{
    return request_invocation->http_request;
}


void mug_request_invocation_set_http_request(mug_request_invocation_t *request_invocation, mug_http_request_t *http_request)
{
    request_invocation->http_request = http_request;
}


void mug_request_invocation_move_http_request(mug_request_invocation_t *request_invocation, mug_http_request_t **dest)
{
    *dest = request_invocation->http_request;
    request_invocation->http_request = NULL;
}