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
static void set_loc_header(char**, const char*);


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

    set_loc_header(&headers->headers[headers->len], header);

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


static void set_loc_header(char **loc, const char *header)
{
    int len = strlen(header);
    *loc = (char*)malloc(len + 1);
    strcpy(*loc, header);
}


static void expand_headers(mug_headers_t *headers)
{
    int new_capacity = headers->capacity * 2;
    char **new_headers = (char**)malloc(sizeof(char*) * new_capacity);

    for (int i = 0; i < headers->len; i++) {
        set_loc_header(&new_headers[i], headers->headers[i]);
    }

    mug_headers_dtor(headers);
    headers->headers = new_headers;
    headers->capacity = new_capacity;
}