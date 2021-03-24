#include "string.h"
#include "unistd.h"
#include "stdio.h"


#include "mug.h"
#include "http_response_writer.h"


void write_response(int sfd, struct mug_response* response)
{
    printf("Write Response\n");
    char status_line[64] = "HTTP/1.1 ";

    /* Write status line */
    switch (response->status_code) {
        case STATUS_OK:
            strcat(status_line, "200 OK\r\n");
            break;
        case STATUS_BAD_REQUEST:
            strcat(status_line, "400 Bad Request\r\n");
            break;
        case STATUS_NOT_FOUND:
            strcat(status_line, "404 Not Found\r\n");
            break;
        case STATUS_INTERNAL_SERVER_ERROR:
            strcat(status_line, "500 Internal Server Error\r\n");
            break;
    }

    write(sfd, status_line, strlen(status_line));

    /* Write headers */
    if (response->headers) {
        for (int i = 0; i < response->headers_size; i++) {
            int header_len = strlen(response->headers[i]);
            write(sfd, response->headers[i], header_len);
            write(sfd, "\r\n", 2);
        }
    }

    /* Write body */
    if (response->body) {
        write(sfd, response->body, response->body_size);
    }
}