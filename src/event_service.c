#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>

#include "event_service.h"
#include "types/misc/mug_http_response.h"
#include "event/request_completed_event.h"
#include "event/event.h"


struct event_service {
    char filepath[256];
};


static int get_connected_socket(const event_service_t *event_service);


event_service_t* event_service_alloc()
{
    return (event_service_t*)malloc(sizeof(event_service_t));
}


void event_service_ctor(event_service_t *event_service, const char *socket_filepath)
{
    strcpy(event_service->filepath, socket_filepath);
}


int event_service_get_fd(const event_service_t *event_service)
{
    // TODO
    return 0;
}


void event_service_raise_request_completed(const event_service_t *event_service, mug_http_response_t* http_response, int fd)
{
    request_completed_event_t *request_completed_event = request_completed_event_alloc();
    request_completed_event_ctor(request_completed_event, fd, http_response);

    int sfd = get_connected_socket(event_service);

    write(sfd, &request_completed_event, sizeof(request_completed_event_t*));

    close(sfd);
}


static int get_connected_socket(const event_service_t *event_service)
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, event_service->filepath);

    connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
    return sfd;
}
