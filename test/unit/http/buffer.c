#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>

#include "http/buffer.h"


#define TEST_SOCKET_FILE_PATH "/tmp/test_sock"
#define BACKLOG 10
#define BUFFER_SIZE 16


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

    buffer_t *buffer = buffer_alloc();
    buffer_ctor_sized(buffer, sockets[1], BUFFER_SIZE);

    state[0] = buffer;
    state[1] = sockets;

    return 0;
}


static int teardown(void **state)
{
    buffer_t *buffer = (buffer_t*)state[0];
    int *sockets = (int*)state[1];

    unlink(TEST_SOCKET_FILE_PATH);
    close(sockets[0]);
    close(sockets[1]);

    buffer_dtor(buffer);
    free(buffer);
    free(sockets);

    return 0;
}


void test_get_char(void **state)
{
    buffer_t *buffer = (buffer_t*)state[0];
    int *sockets = (int*)state[1];

    const char *str = "test";
    size_t buf_len = strlen(str) + 1;

    int clientfd = accept(sockets[0], NULL, NULL);
    write(clientfd, str, buf_len);

    char *buf = (char*)malloc(sizeof(char) * buf_len);
    for (int i = 0; i < buf_len; i++) {
        buf[i] = buffer_get_char(buffer);
    }

    assert_true(strcmp(str, buf) == 0);

    free(buf);
}


void test_get_char_oversize(void **state)
{
    buffer_t *buffer = (buffer_t*)state[0];
    int *sockets = (int*)state[1];

    const char *str = "This is a long test message!";
    size_t buf_len = strlen(str) + 1;
    assert_true(buf_len > BUFFER_SIZE);

    int clientfd = accept(sockets[0], NULL, NULL);
    write(clientfd, str, buf_len);

    char *buf = (char*)malloc(sizeof(char) * buf_len);
    for (int i = 0; i < buf_len; i++) {
        buf[i] = buffer_get_char(buffer);
    }

    assert_true(strcmp(str, buf) == 0);

    free(buf);
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_get_char, setup, teardown),
        cmocka_unit_test_setup_teardown(test_get_char_oversize, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}