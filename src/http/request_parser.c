#include <stdlib.h>
#include <string.h>

#include "http/request_parser.h"
#include "http/buffer.h"
#include "types/misc/mug_http_request.h"
#include "types/misc/mug_body.h"
#include "types/misc/mug_http_method.h"
#include "types/misc/mug_http_version.h"


#define DEFAULT_BODY_BUF_SIZE 1024
#define MAX_METHOD_LEN 8
#define MAX_VERSION_LEN 16
#define MAX_HEADER_LEN 256


struct request_parser {
    buffer_t *buffer;
};


static void request(buffer_t*, mug_http_request_t*);
static void headers(buffer_t*, mug_http_request_t*);
static void body(buffer_t*, mug_http_request_t*);
static void match_method(buffer_t*, mug_http_request_t*);
static void match_url(buffer_t*, mug_http_request_t*);
static void match_version(buffer_t*, mug_http_request_t*);
static int match_header(buffer_t*, mug_http_request_t*);
static int buffer_read_to_space(buffer_t*, char*, size_t);
static int buffer_read_to_rn(buffer_t*, char*, size_t);


request_parser_t* request_parser_alloc()
{
    return (request_parser_t*)malloc(sizeof(request_parser_t));
}


void request_parser_ctor(request_parser_t *request_parser, int sfd)
{
    buffer_t *buffer = buffer_alloc();
    buffer_ctor(buffer, sfd);

    request_parser->buffer = buffer;
}


void request_parser_dtor(request_parser_t *request_parser)
{
    buffer_dtor(request_parser->buffer);
    free(request_parser->buffer);
}


mug_http_request_t* request_parser_parse(request_parser_t *request_parser)
{
    mug_http_request_t *http_request = mug_http_request_alloc();
    mug_http_request_ctor(http_request);

    request(request_parser->buffer, http_request);

    return http_request;
}


static void request(buffer_t *buffer, mug_http_request_t *http_request)
{
    match_method(buffer, http_request);
    match_url(buffer, http_request);
    match_version(buffer, http_request);
    headers(buffer, http_request);
    body(buffer, http_request);
}


static void headers(buffer_t *buffer, mug_http_request_t *http_request)
{
    while (match_header(buffer, http_request));
}


static void body(buffer_t *buffer, mug_http_request_t *http_request)
{
    mug_body_t *body = mug_http_request_body(http_request);

    char ch;
    while ((ch = buffer_get_char(buffer)) != '\0') {
        mug_body_append_content(body, &ch, sizeof(char));
    }
}


static void match_method(buffer_t *buffer, mug_http_request_t *http_request)
{
    char method_buf[MAX_METHOD_LEN];

    buffer_read_to_space(buffer, method_buf, MAX_METHOD_LEN);

    if (strcmp(method_buf, "GET") == 0) {
        mug_http_request_set_http_method(http_request, HTTP_GET);
    }
    else if (strcmp(method_buf, "HEAD") == 0) {
        mug_http_request_set_http_method(http_request, HTTP_HEAD);
    }
    else if (strcmp(method_buf, "POST") == 0) {
        mug_http_request_set_http_method(http_request, HTTP_POST);
    }
    else if (strcmp(method_buf, "PUT") == 0) {
        mug_http_request_set_http_method(http_request, HTTP_PUT);
    }
    else if (strcmp(method_buf, "DELETE") == 0) {
        mug_http_request_set_http_method(http_request, HTTP_DELETE);
    }
    else {
        mug_http_request_set_http_method(http_request, HTTP_METHOD_UNKNOWN);
    }
}


static void match_url(buffer_t *buffer, mug_http_request_t *http_request)
{
    char url_buf[MAX_URL_LEN];

    buffer_read_to_space(buffer, url_buf, MAX_URL_LEN);

    mug_http_request_set_http_url(http_request, url_buf);
}


static void match_version(buffer_t *buffer, mug_http_request_t *http_request)
{
    char version_buf[MAX_VERSION_LEN];

    int nread = buffer_read_to_rn(buffer, version_buf, MAX_VERSION_LEN);

    version_buf[nread - 2] = '\0';  /* Strip \r\n */

    if (strcmp(version_buf, "HTTP/1.0") == 0) {
        mug_http_request_set_http_version(http_request, HTTP_1_0);
    }
    else if (strcmp(version_buf, "HTTP/1.1") == 0) {
        mug_http_request_set_http_version(http_request, HTTP_1_1);
    }
    else if (strcmp(version_buf, "HTTP/2") == 0) {
        mug_http_request_set_http_version(http_request, HTTP_2);
    }
}


static int match_header(buffer_t *buffer, mug_http_request_t *http_request)
{
    char header_buf[MAX_HEADER_LEN];
    
    int nread = buffer_read_to_rn(buffer, header_buf, MAX_HEADER_LEN);

    if (strcmp(header_buf, "\r\n") == 0) {
        return 0;  /* False - Read last header */
    }

    header_buf[nread - 2] = '\0';  /* Strip \r\n */

    mug_headers_t *headers = mug_http_request_headers(http_request);
    mug_headers_append(headers, header_buf);

    return 1; /* True - Continue reading headers */
}


static int buffer_read_to_space(buffer_t *buffer, char *buf, size_t len)
{
    int nread = 1;

    for (int i = 0; i < len; i++, nread++) {
        char ch = buffer_get_char(buffer);

        if (ch == ' ') {
            break;
        }

        buf[i] = ch;
    }

    buf[nread - 1] = '\0';

    return nread;
}


static int buffer_read_to_rn(buffer_t *buffer, char *buf, size_t len)
{
    int carriage_return_seen = 0;
    int nread = 1;

    for (int i = 0; i < len; i++, nread++) {
        char ch = buffer_get_char(buffer);

        buf[i] = ch;

        if (ch == '\r') {
            carriage_return_seen = 1;
        }

        if (ch == '\n' && carriage_return_seen == 1) {
            break;
        }
    }

    int end_of_str = nread < len ? nread : len - 1;
    buf[end_of_str] = '\0';

    return nread;
}
