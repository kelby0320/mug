#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "http/request_parser.h"
#include "types/misc/mug_http_request.h"
#include "types/misc/mug_http_method.h"
#include "types/misc/mug_http_version.h"
#include "types/misc/mug_headers.h"


#define TEST_SOCKET_FILE_PATH "/tmp/test_sock"
#define BACKLOG 10


static int create_listening_socket()
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, TEST_SOCKET_FILE_PATH);

    bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
    listen(sfd, BACKLOG);

    return sfd;
}


static int get_connected_socket()
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, TEST_SOCKET_FILE_PATH);

    connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));

    return sfd;
}


static int setup(void **state)
{
    int *sockets = (int*)malloc(sizeof(int*) * 2);
    sockets[0] = create_listening_socket();
    sockets[1] = get_connected_socket();

    request_parser_t *parser = request_parser_alloc();
    request_parser_ctor(parser, sockets[1]);

    state[0] = parser;
    state[1] = sockets;

    return 0;
}


static int teardown(void **state)
{
    request_parser_t *parser = (request_parser_t*)state[0];
    int *sockets = (int*)state[1];

    unlink(TEST_SOCKET_FILE_PATH);
    close(sockets[0]);
    close(sockets[1]);

    request_parser_dtor(parser);
    free(parser);
    free(sockets);

    return 0;
}


void test_get_request(void **state)
{
    const char *req = "GET /index.html HTTP/1.1\r\n"
                      "Host: 127.0.0.1\r\n"
                      "\r\n";
    size_t req_len = strlen(req) + 1;

    request_parser_t *parser = (request_parser_t*)state[0];
    int *sockets = (int*)state[1];

    int clientfd = accept(sockets[0], NULL, NULL);
    write(clientfd, req, req_len);

    mug_http_request_t *http_request = request_parser_parse(parser);

    mug_http_method_t method =  mug_http_request_http_method(http_request);
    assert_true(method == HTTP_GET);

    char url_buf[16];
    mug_http_request_http_url(http_request, url_buf);
    assert_true(strcmp(url_buf, "/index.html") == 0);

    mug_http_version_t version = mug_http_request_http_version(http_request);
    assert_true(version == HTTP_1_1);

    mug_headers_t *headers = mug_http_request_headers(http_request);
    int headers_len = mug_headers_len(headers);
    assert_true(headers_len == 1);


    char header_buf[32];
    mug_headers_get(headers, 0, header_buf);
    assert_true(strcmp(header_buf, "Host: 127.0.0.1") == 0);

    mug_body_t *body = mug_http_request_body(http_request);
    assert_true(mug_body_len(body) == 0);
}


void test_post_request(void **state)
{
    const char *req = "POST /post.html HTTP/1.1\r\n"
                      "Host: 127.0.0.1\r\n"
                      "Content-Length: 18\r\n"
                      "\r\n"
                      "{ 'json': 'body' }";
    size_t req_len = strlen(req) + 1;

    request_parser_t *parser = (request_parser_t*)state[0];
    int *sockets = (int*)state[1];

    int clientfd = accept(sockets[0], NULL, NULL);
    write(clientfd, req, req_len);

    mug_http_request_t *http_request = request_parser_parse(parser);

    mug_http_method_t method =  mug_http_request_http_method(http_request);
    assert_true(method == HTTP_POST);

    char url_buf[16];
    mug_http_request_http_url(http_request, url_buf);
    assert_true(strcmp(url_buf, "/post.html") == 0);

    mug_http_version_t version = mug_http_request_http_version(http_request);
    assert_true(version == HTTP_1_1);

    mug_headers_t *headers = mug_http_request_headers(http_request);
    int headers_len = mug_headers_len(headers);
    assert_true(headers_len == 2);


    char header_buf[32];
    mug_headers_get(headers, 0, header_buf);
    assert_true(strcmp(header_buf, "Host: 127.0.0.1") == 0);

    mug_headers_get(headers, 1, header_buf);
    assert_true(strcmp(header_buf, "Content-Length: 18") == 0);

    mug_body_t *body = mug_http_request_body(http_request);
    assert_true(mug_body_len(body) == 18);

    char body_buf[32];
    mug_body_as_c_str(body, body_buf, 32);
    assert_true(strcmp(body_buf, "{ 'json': 'body' }") == 0);
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_get_request, setup, teardown),
        cmocka_unit_test_setup_teardown(test_post_request, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}