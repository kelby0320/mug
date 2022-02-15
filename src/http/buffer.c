#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>

#include "http/buffer.h"


#define DEFAULT_BUFFER_CAPACITY 1024


struct buffer {
    int sfd;
    char *buf;
    size_t capacity;
    unsigned int filled;
    unsigned int loc;
    unsigned int read_complete;
};


static void load_chunk(buffer_t*);
static int end_of_buffer(buffer_t*);


buffer_t* buffer_alloc()
{
    return (buffer_t*)malloc(sizeof(buffer_t));
}


void buffer_ctor(buffer_t *buffer, int sfd)
{
    buffer->sfd = sfd;
    buffer->buf = (char*)malloc(sizeof(char) * DEFAULT_BUFFER_CAPACITY);
    buffer->capacity = DEFAULT_BUFFER_CAPACITY;
    buffer->filled = 0;
    buffer->loc = 0;
    buffer->read_complete = 0;  /* False */
}


void buffer_ctor_sized(buffer_t *buffer, int sfd, size_t cap)
{
    buffer->sfd = sfd;
    buffer->buf = (char*)malloc(sizeof(char) * cap);
    buffer->capacity = cap;
    buffer->filled = 0;
    buffer->loc = 0;
}


void buffer_dtor(buffer_t *buffer)
{
    free(buffer->buf);
}


char buffer_get_char(buffer_t *buffer)
{
    if (end_of_buffer(buffer)) {
        if (!buffer->read_complete) {
            load_chunk(buffer);
        }
        else {
            return '\0';
        }
        
    }

    char c = buffer->buf[buffer->loc];
    buffer->loc++;
    return c;
}


static void load_chunk(buffer_t *buffer)
{
    int nread = read(buffer->sfd, (void*)buffer->buf, buffer->capacity);

    buffer->filled = nread;
    buffer->loc = 0;

    if (nread < buffer->capacity) {
        buffer->read_complete = 1;  /* True */
    }
}

static int end_of_buffer(buffer_t *buffer)
{
    return buffer->loc == buffer->filled;
}