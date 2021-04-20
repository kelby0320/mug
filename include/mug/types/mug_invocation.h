#ifndef MUG_INVOCATION_H
#define MUG_INVOCATION_H


typedef enum {
    MUG_INVOCATION_BASE,
    MUG_REQUEST_INVOCATION,
    MUG_CONTINUATION_INVOCATION,
    MUG_HTTP_CONTINUATION_INVOCATION
} mug_invocation_type_t;


typedef struct {
    mug_invocation_type_t type;
} mug_invocation_t;


#endif