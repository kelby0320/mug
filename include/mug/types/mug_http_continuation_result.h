#ifndef MUG_HTTP_CONTINUATION_RESULT_H
#define MUG_HTTP_CONTINUATION_RESULT_H


#include "types/mug_continuation_result.h"
#include "types/mug_http_request.h"

typedef struct {
    mug_continuation_result_t continuation_result;
    struct mug_http_request http_request;
} mug_http_continuation_result_t;


#endif