#ifndef MUG_RESULT_H
#define MUG_RESULT_H


typedef enum {
    MUG_RESULT_BASE,
    MUG_RESPONSE_RESULT,
    MUG_CONTINUATION_RESULT,
    MUG_HTTP_CONTINUATION_RESULT
} mug_result_type_t;


typedef struct {
    mug_result_type_t type;
} mug_result_t;


#endif