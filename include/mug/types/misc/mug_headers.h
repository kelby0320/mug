#ifndef MUG_HEADERS_H
#define MUG_HEADERS_H


struct mug_headers;


typedef struct mug_headers mug_headers_t;


mug_headers_t* mug_headers_alloc();
void mug_headers_ctor(mug_headers_t*);
void mug_headers_dtor(mug_headers_t*);
void mug_headers_append(mug_headers_t*, const char*);
void mug_headers_get(const mug_headers_t*, int, char*);
int mug_headers_len(const mug_headers_t*);


#endif