#ifndef HTTP_PARSER_H
#define HTTP_PARSE_H


#include "mug.h"


struct mug_request* parse_http_request(int sfd);


#endif