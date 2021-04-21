#ifndef MUG_INVOCATION_TYPE_H
#define MUG_INVOCATION_TYPE_H


typedef enum {
    MUG_INVOCATION_BASE,
    MUG_REQUEST_INVOCATION,
    MUG_CONTINUATION_INVOCATION_BASE,
    MUG_HTTP_CONTINUATION_INVOCATION
} mug_invocation_type_t;


#endif