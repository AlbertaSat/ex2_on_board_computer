
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "utils.h"
#include "packet.h"
#include "mib.h"

typedef struct response {
    int sfd;
    char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    //getting rid of this soon in favour of client based packet sizes
    uint32_t packet_len;
    
} Response;

typedef enum Indication {
    none,
    transaction,
    put
} Indication;

typedef struct request {
    Indication type;
    uint32_t transaction_id;
    uint32_t dest_cfdp_id;
    char *source_file_name;
    char *destination_file_name;
    uint8_t segmentation_control;
    uint8_t fault_handler_overides;
    uint8_t flow_lable;
    uint8_t transmission_mode;
    char* messages_to_user;
    char* filestore_requests;
    char* buff;
} Request;


typedef struct client {
    
    void *client_handle;
    void *client_thread_attributes;
    uint32_t packet_len;

    //this is a queue of user requests that will be going out
    Request *outGoing_req;
    Request *incoming_req;

    uint32_t cfdp_id;
    uint32_t unitdata_id;
    uint32_t unitdata_port;
    
    Remote_entity *mib_info;
    Pdu_header *pdu_header;
    
    uint8_t is_active;

} Client;


typedef struct protocol_state {
    uint32_t packet_size;
    char *server_port;
    void *server_handle;
    void *server_thread_attributes;
    MIB *mib;
    uint32_t my_cfdp_id;
    Client *newClient;
    Request *current_server_request;


} Protocol_state;


void packet_handler_server(Response res, Request *req, Protocol_state *p_state);
void parse_packet_server(char* buff, Request *req, Protocol_state *p_state);
void packet_handler_client(Response res, Request *req, Client *client, Protocol_state *p_state);
void user_request_handler(Response res, Request *req, Client *client, Protocol_state *p_state);
void parse_packet_client(char* buff, Request *req, Client *client, Protocol_state *p_state);
void build_pdu_header(char* packet, Request *req, Client* client, Protocol_state *p_state);
#endif