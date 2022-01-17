#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "http/buffer.h"


#define DEFAULT_BUFFER_CAPACITY 1024


struct buffer {
    int sfd;
    char *buf;
    size_t capacity;
    unsigned int filled;
    unsigned int loc;
};


static void load_chunk(buffer_t*);


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
    if (buffer->loc == buffer->filled) {
        load_chunk(buffer);
    }

    if (buffer->filled == 0) {
        return '\0';
    }

    char c = buffer->buf[buffer->loc];
    buffer->loc++;
    return c;
}


static void load_chunk(buffer_t *buffer)
{
    int sfd = buffer->sfd;
    void *buf = (void*)buffer->buf;
    size_t count = buffer->capacity;

    int nread = read(sfd, buf, count);

    buffer->filled = nread;
    buffer->loc = 0;
}