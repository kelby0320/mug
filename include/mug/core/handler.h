#ifndef HANDLER_H
#define HANDLER_H


#include "types/result/mug_result.h"
#include "types/invocation/mug_request_invocation.h"
#include "types/invocation/mug_continuation_invocation.h"


typedef mug_result_t* (*mug_request_handler_t)(mug_request_invocation_t*);
typedef mug_result_t* (*mug_continuation_handler_t)(mug_continuation_invocation_t*);


#endif