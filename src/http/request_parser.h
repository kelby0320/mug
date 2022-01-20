#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H


#include "types/misc/mug_http_request.h"


struct request_parser;


typedef struct request_parser request_parser_t;


request_parser_t* request_parser_alloc();
void request_parser_ctor(request_parser_t*, int);
void request_parser_dtor(request_parser_t*);
mug_http_request_t* request_parser_parse(request_parser_t*);


#endif