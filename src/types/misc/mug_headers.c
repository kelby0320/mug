#include <stdlib.h>
#include <string.h>

#include "types/misc/mug_headers.h"


#define DEFAULT_HEADERS_CAPACITY 16


struct mug_headers {
    char **headers;
    int capacity;
    int len;
};


mug_headers_t* mug_headers_alloc()
{
    return (mug_headers_t*)malloc(sizeof(mug_headers_t));
}


static void expand_headers(mug_headers_t*);


void mug_headers_ctor(mug_headers_t *headers)
{
    headers->headers = (char**)malloc(sizeof(char*) * DEFAULT_HEADERS_CAPACITY);
    headers->capacity = DEFAULT_HEADERS_CAPACITY;
    headers->len = 0;
}


void mug_headers_dtor(mug_headers_t *headers)
{
    for (int i = 0; i < headers->len; i++) {
        free(headers->headers[i]);
    }
    
    free(headers->headers);
}


void mug_headers_append(mug_headers_t *headers, const char *header)
{
    if (headers->len == headers->capacity) {
        expand_headers(headers);
    }

    char *loc = headers->headers[headers->len];

    int header_len = strlen(header);
    loc = (char*)malloc(header_len + 1);
    strcpy(loc, header);

    headers->len++;
}


void mug_headers_get(const mug_headers_t *headers, int loc, char *out)
{
    if (loc < 0) {
        loc = 0;
    } else if (loc >= headers->len) {
        loc = headers->len - 1;
    }

    strcpy(out, headers->headers[loc]);
}


int mug_headers_len(const mug_headers_t *headers)
{
    return headers->len;
}