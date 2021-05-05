#ifndef MUG_BODY_H
#define MUG_BODY_H


struct mug_body;


typedef struct mug_body mug_body_t;


mug_body_t* mug_body_alloc();
void mug_body_ctor(mug_body_t*);
void mug_body_dtor(mug_body_t*);
void mug_body_set_content(mug_body_t*, const void*, size_t);
void mug_body_set_content_str(mug_body_t*, const char*);
void mug_body_append_content(mug_body_t*, const void*, size_t);
void mug_body_append_content_str(mug_body_t*, const char*);
int mug_body_len(const mug_body_t*);
int mug_body_as_c_str(const mug_body_t*, char*, size_t);
int mug_body_as_bytes(const mug_body_t*, void*, size_t);


#endif