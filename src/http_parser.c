#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "mug.h"
#include "http_parser.h"


#define REQUEST_BUFFER_SIZE 8196


static int request_line(int, char*, int*, int*, struct mug_request*);
// static void headers(int, char*, int*, int*, struct mug_request*);
// static void body(int, char*, int*, int*, struct mug_request*);
static int fill_request_buffer(int, char*);
static int match_request_method(char*, int, int, mug_http_method_t*);


struct mug_request* parse_http_request(int sfd)
{
    char *buf = (char*)malloc(REQUEST_BUFFER_SIZE);
    int buf_len = 0;
    int buf_loc = 0;
    struct mug_request *mug_request = (struct mug_request*)malloc(sizeof(struct mug_request));

    request_line(sfd, buf, &buf_loc, &buf_len, mug_request);
    // headers(sfd, buf, &buf_loc, mug_request);
    // body(sfd, buf, &buf_loc, mug_request);

    return mug_request;
}


static int request_line(int sfd, char* buf, int *buf_loc, int *buf_len, struct mug_request* mug_request)
{
    *buf_len = fill_request_buffer(sfd, buf);
    int token_start = 0;
    int token_end;

    /* Parse Request Method */
    for (token_end = token_start; buf[token_end] != ' ' && token_end < *buf_len; token_end++);
    mug_http_method_t request_method;
    if (match_request_method(buf, token_start, token_end, &request_method) != 0) {
        return -1;
    }
    mug_request->req_method = request_method;

    /* Parse URL */
    token_start = ++token_end;
    for (;buf[token_end] != ' ' && token_end < *buf_len; token_end++);
    int url_size = token_end - token_start;
    char *url = (char*)calloc(url_size + 1, sizeof(char));
    strncpy(url, buf + token_start, url_size);
    mug_request->url = url;

    /* Parse HTTP version */
    token_start = ++token_end;
    for (;buf[token_end] != '\r' && token_end < *buf_len; token_end++);
    if (token_end + 1 < *buf_len && buf[token_end + 1] != '\n') {
        return -1;
    }
}


static int fill_request_buffer(int sfd, char *buf)
{
    int num_read = read(sfd, buf, REQUEST_BUFFER_SIZE);
    return num_read;
}


static int match_request_method(char *buf, int start, int end, mug_http_method_t *request_method)
{
    int size = end - start;

    if (strncmp(buf + start, "GET", size) == 0) {
        *request_method = HTTP_GET;
        return 0;
    }
    else if (strncmp(buf + start, "HEAD", size) == 0) {
        *request_method = HTTP_HEAD;
        return 0;
    }
    else if (strncmp(buf + start, "POST", size) == 0) {
        *request_method = HTTP_POST;
        return 0;
    }
    else if (strncmp(buf + start, "PUT", size) == 0) {
        *request_method = HTTP_PUT;
        return 0;
    }
    else if (strncmp(buf + start, "DELETE", size) == 0) {
        *request_method = HTTP_DELETE;
        return 0;
    }
    else {
        return -1;
    }
}
