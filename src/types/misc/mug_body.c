#include <stdlib.h>
#include <string.h>

#include "types/misc/mug_body.h"


#define DEFAULT_BODY_BUF_SIZE 128


struct mug_body {
    char *buffer;
    int len;
    int capacity;
};


static int copy_buffer(void*, size_t, void*, size_t);
static void resize_buffer(mug_body_t*, size_t);
static void set_buffer_content(mug_body_t*, const void*, size_t);
static void append_buffer_content(mug_body_t*, const void*, size_t);


mug_body_t* mug_body_alloc()
{
    return (mug_body_t*)malloc(sizeof(mug_body_t));
}


void mug_body_ctor(mug_body_t *mug_body)
{
    mug_body->buffer = (char*)malloc(sizeof(char) * DEFAULT_BODY_BUF_SIZE);
    mug_body->capacity = DEFAULT_BODY_BUF_SIZE;
    mug_body->len = 0;
}


void mug_body_dtor(mug_body_t *mug_body)
{
    free(mug_body->buffer);
}


void mug_body_set_content(mug_body_t *mug_body, const void *content, size_t size)
{
    set_buffer_content(mug_body, content, size);

    mug_body->len = size;
}


void mug_body_set_content_str(mug_body_t *mug_body, const char *content)
{
    int len = strlen(content);

    set_buffer_content(mug_body, content, len);

    mug_body->len = len;
}


void mug_body_append_content(mug_body_t *mug_body, const void *content, size_t size)
{
    append_buffer_content(mug_body, content, size);

    mug_body->len += size;
}


void mug_body_append_content_str(mug_body_t *mug_body, const char *content)
{
    int len = strlen(content);

    append_buffer_content(mug_body, content, len);

    mug_body->len += len;
}


int mug_body_len(const mug_body_t *mug_body)
{
    return mug_body->len;
}


int mug_body_as_c_str(const mug_body_t *mug_body, char *out, size_t size)
{
    int copied = copy_buffer(out, size - 1, mug_body->buffer, mug_body->len);

    char null_terminator = '\0';
    memcpy(out + size - 1, &null_terminator, 1);

    return ++copied;
}


int mug_body_as_bytes(const mug_body_t *mug_body, void *out, size_t size)
{
    return copy_buffer(out, size, mug_body->buffer, mug_body->len);
}


static int copy_buffer(void *dest, size_t dest_size, void *src, size_t src_size)
{
    int to_copy = src_size;
    if (dest_size < to_copy) {
        to_copy = dest_size;
    }

    memcpy(dest, src, to_copy);

    return to_copy; 
}


static void resize_buffer(mug_body_t *mug_body, size_t size)
{
    char *new_buffer = (char*)malloc(sizeof(char) * size);

    memcpy(new_buffer, mug_body->buffer, mug_body->len);

    free(mug_body->buffer);
    mug_body->buffer = new_buffer;
    mug_body->capacity = size;
}


static void set_buffer_content(mug_body_t *mug_body, const void *content, size_t size)
{
    if (mug_body->capacity < size) {
        resize_buffer(mug_body, size);
    }

    memcpy(mug_body->buffer, content, size);
}


static void append_buffer_content(mug_body_t *mug_body, const void *content, size_t size)
{
    int pad = mug_body->capacity - mug_body->len;

    if (pad < size) {
        int new_size = mug_body->len + size;
        resize_buffer(mug_body, new_size);
    }

    char *loc = mug_body->buffer + mug_body->len;
    memcpy(loc, content, size);
}