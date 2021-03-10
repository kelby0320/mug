#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


#include "mug.h"
#include "http_parser.h"


#define REQUEST_BUFFER_SIZE 8196


static int request_line(int, char*, int*, int*, struct mug_request*);
static int headers(int, char*, int*, int*, struct mug_request*);
// static void body(int, char*, int*, int*, struct mug_request*);
static int fill_request_buffer(int, char*);
static int match_request_method(char*, int, int, mug_http_method_t*);
static int append_header(char*, int, struct mug_request*);


struct mug_request* parse_http_request(int sfd)
{
    char *buf = (char*)malloc(REQUEST_BUFFER_SIZE);
    int buf_len = 0;
    int buf_loc = 0;
    struct mug_request *mug_request = (struct mug_request*)calloc(1, sizeof(struct mug_request));

    request_line(sfd, buf, &buf_loc, &buf_len, mug_request);
    headers(sfd, buf, &buf_loc, &buf_len, mug_request);
    // body(sfd, buf, &buf_loc, &buf_len, mug_request);

    return mug_request;
}


static int request_line(int sfd, char* buf, int *buf_loc, int *buf_len, struct mug_request *mug_request)
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

    *buf_loc = token_end + 2;

    return 0;
}

static int headers(int sfd, char *buf, int *buf_loc, int *buf_len, struct mug_request *mug_request)
{
    int header_start = *buf_loc;
    int header_end;

    for (int i = 0; i < 5; i++) {
        for (header_end = header_start; buf[header_end] != '\r' && header_end < *buf_len; header_end++);
        if (header_end + 1 < *buf_len && buf[header_end + 1] != '\n') {
            return -1;
        }
        int header_len = header_end - header_start;

        /* Stop when we read a line consisting of only "\r\n" */
        if (header_len == 0 && 
            buf[header_start] == '\r' && 
            buf[header_start + 1] == '\n')
        {
            break;
        }

        append_header(buf + header_start, header_len, mug_request);
        header_start = header_end + 2;
    }
    
    return 0;
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


static int append_header(char *buf, int header_len, struct mug_request *mug_request)
{
    /* 
     * Case 1: No headers allocated 
     */
    if (mug_request->headers == NULL) {
        mug_request->headers = (char**)malloc(sizeof(char*));
        mug_request->headers[0] = (char*)calloc(header_len + 1, sizeof(char));
        strncpy(mug_request->headers[0], buf, header_len);
        mug_request->headers_size = 1;
        return mug_request->headers_size;
    }

    /* 
     * Case 2: Append header to existing headers array 
     */

    /* Allocate new headers array */
    int new_headers_size = mug_request->headers_size + 1;
    char **new_headers = (char**)malloc(sizeof(char*) * new_headers_size);

    /* Copy existing items */
    for (int i = 0; i < mug_request->headers_size; i++) {
        int len = strlen(mug_request->headers[i]);
        new_headers[i] = (char*)malloc(sizeof(char) * (len + 1));
        strcpy(new_headers[i], mug_request->headers[i]);
    }

    /* Append new header */
    new_headers[new_headers_size - 1] = (char*)calloc(header_len + 1, sizeof(char));
    strncpy(new_headers[new_headers_size - 1], buf, header_len);

    /* Free old headers */
    for (int i = 0; i < mug_request->headers_size; i++) {
        free(mug_request->headers[i]);
    }
    free(mug_request->headers);

    /* Swap old headers for new headers */
    mug_request->headers = new_headers;
    mug_request->headers_size = new_headers_size;
    return mug_request->headers_size;
}
