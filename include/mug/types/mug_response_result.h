#ifndef MUG_RESPONSE_RESULT_H
#define MUG_RESPONSE_RESULT_H


#include "types/mug_result.h"
#include "types/mug_http_response.h"


typedef struct {
    mug_result_t result;
    mug_http_response_t http_response;
} mug_response_result_t;

#endif