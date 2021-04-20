#ifndef HANDLER_H
#define HANDLER_H


#include "types/mug_result.h"
#include "types/mug_request_invocation.h"
#include "types/mug_continuation_invocation.h"


typedef mug_result_t* (*request_handler_t)(mug_request_invocation_t*);
typedef mug_result_t* (*continuation_handler_t)(mug_continuation_invocation_t*);


#endif