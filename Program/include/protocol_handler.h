
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "utils.h"

typedef struct response {
    int sfd;
    char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    int packet_len;
} Response;


typedef struct client {
    char *host_name;
    char *client_port;
    void *client_handle;
    void *client_thread_attributes;
    int is_active;
} Client;


typedef struct protocol_state {
    int packet_size;
    char *server_port;
    void *server_handle;
    void *server_thread_attributes;
    List *client_list;

    //temperary until client list works fully
    Client *client;

} Protocol_state;



void packet_handler_server(Response res);
void packet_handler_client(Response res);


#endif