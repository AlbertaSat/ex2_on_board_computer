
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "utils.h"
#include "packet.h"

typedef struct response {
    int sfd;
    char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    //getting rid of this soon in favour of client based packet sizes
    uint32_t packet_len;
} Response;


typedef struct request {
    uint8_t request_primitive;
    uint32_t transaction_id;
} Request;


typedef struct client {
    char *host_name;
    char *client_port;
    void *client_handle;
    void *client_thread_attributes;
    uint32_t packet_len;
    uint8_t is_active;

} Client;


typedef struct protocol_state {
    uint32_t packet_size;
    char *server_port;
    void *server_handle;
    void *server_thread_attributes;

    List *client_list;
    //temperary until client list works fully
    Client *client;

} Protocol_state;


int cleanup_list(void *element);
int list_remove(void *element, void *args);
void list_print(void *element, void *args);
int list_find(void *element, void *args);
void list_free(void *element);



void packet_handler_server(Response res, Protocol_state *p_state);
void packet_handler_client(Response res, Protocol_state *p_state);


#endif