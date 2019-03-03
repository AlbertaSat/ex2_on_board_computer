
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

typedef enum Indication {
    transaction,
    put
} Indication;

typedef struct request {
    Indication type;
    uint32_t transaction_id;
    char *dest_cfdp_id;
    char *source_file_name;
    char *destination_file_name;
    uint8_t segmentation_control;
    uint8_t fault_handler_overides;
    uint8_t flow_lable;
    uint8_t transmission_mode;
    char* messages_to_user;
    char* filestore_requests;
} Request;




typedef struct client {
    char *host_name;
    char *client_port;
    void *client_handle;
    void *client_thread_attributes;
    uint32_t packet_len;

    //this is a queue of requests that will be processed
    List *requests;
    uint8_t is_active;


} Client;


typedef struct protocol_state {
    uint32_t packet_size;
    char *server_port;
    void *server_handle;
    void *server_thread_attributes;

    List *client_list;
    Client *newClient;
} Protocol_state;


void packet_handler_server(Response res, Request req, Protocol_state *p_state);
void packet_handler_client(Response res, Request req, Protocol_state *p_state);
void parse_packet(char* buff, Request req, Protocol_state *p_state);

#endif