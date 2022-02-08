#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "event_handler/event_handler.h"
#include "event/internal_event.h"
#include "event/request_completed_event.h"
#include "types/misc/mug_http_response.h"
#include "types/misc/mug_http_version.h"
#include "types/misc/mug_http_status_code.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"
#include "core/handler.h"


static void write_http_response(const mug_http_response_t*, int);
static void write_http_status_line(const mug_http_response_t*, int);
static void write_http_headers(const mug_http_response_t*, int);
static void write_http_body(const mug_http_response_t*, int);


void handle_request_completed(void *arg)
{
    struct event_handler_params *params = (struct event_handler_params*)arg;
    const request_completed_event_t *event = (request_completed_event_t*)params->event;

    mug_http_response_t *http_response = request_completed_event_http_response(event);
    int sfd = request_completed_event_fd(event);

    write_http_response(http_response, sfd);

    destroy_event_handler_params(params);
    free(params);
}


static void write_http_response(const mug_http_response_t *http_response, int sfd)
{
    write_http_status_line(http_response, sfd);
    write_http_headers(http_response, sfd);
    write_http_body(http_response, sfd);
}


static void write_http_status_line(const mug_http_response_t *http_response, int sfd)
{
    mug_http_version_t http_version = mug_http_response_http_version(http_response);
    mug_http_status_code_t status_code = mug_http_response_status_code(http_response);

    char msg[128] = "";
    mug_http_response_message(http_response, msg);

    switch (http_version) {
        case HTTP_1_0:
            write(sfd, "HTTP/1.0", 8);
            break;
        case HTTP_1_1:
            write(sfd, "HTTP/1.1", 8);
            break;
        case HTTP_2:
            write(sfd, "HTTP/2", 6);
            break;
    }

    switch (status_code) {
        case STATUS_OK:
            write(sfd, "200", 3);
            break;
        case STATUS_BAD_REQUEST:
            write(sfd, "400", 3);
            break;
        case STATUS_NOT_FOUND:
            write(sfd, "404", 3);
            break;
        case STATUS_INTERNAL_SERVER_ERROR:
            write(sfd, "500", 3);
            break;
    }

    int msg_len = strlen(msg);
    if (msg_len > 0) {
        write(sfd, msg, msg_len);
    }

    write(sfd, "\r\n", 2);
}

static void write_http_headers(const mug_http_response_t *http_response, int sfd)
{
    mug_headers_t *headers = mug_http_response_headers(http_response);

    char header_buf[256];
    int headers_len = mug_headers_len(headers);

    for (int i = 0; i < headers_len; i++) {
        mug_headers_get(headers, i, header_buf);
        int hlen = strlen(header_buf);
        write(sfd, header_buf, hlen);
        write(sfd, "\r\n", 2);
    }

    write(sfd, "\r\n", 2);
}


static void write_http_body(const mug_http_response_t *http_response, int sfd)
{
    mug_body_t *body = mug_http_response_body(http_response);
    int body_len = mug_body_len(body);

    if (body_len > 0) {
        char *buf = (char*)malloc(sizeof(char) * body_len);
        mug_body_as_bytes(body, buf, body_len);
        write(sfd, buf, body_len);
        free(buf);
    }
}