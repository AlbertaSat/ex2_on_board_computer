
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "utils.h"
#include "packet.h"
#include "mib.h"
#include "filesystem_funcs.h"

typedef struct response {
    int sfd;
    unsigned char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    //getting rid of this soon in favour of client based packet sizes
    uint32_t packet_len;
    
} Response;

typedef enum Indication {
    none,
    transaction,
    put,

    //sending data
    sending_data

} Indication;

typedef struct request {
    Indication type;
    uint32_t transaction_id;
    uint32_t dest_cfdp_id;

    File *file;
    uint32_t file_size;

    unsigned char *source_file_name;
    unsigned char *destination_file_name;

    uint8_t segmentation_control;
    uint8_t fault_handler_overides;
    uint8_t flow_lable;
    uint8_t transmission_mode;

    unsigned char* messages_to_user;
    unsigned char* filestore_requests;
    
    uint32_t buff_len;
    unsigned char* buff;
} Request;


typedef struct client {
    
    void *client_handle;
    void *client_thread_attributes;
    uint32_t packet_len;

    //this is a queue of user requests that will be going out
    Request *outGoing_req;
    Request *incoming_req;

    //can get rid of these, these are found in mib_info anyway
    uint32_t cfdp_id;
    uint32_t unitdata_id;
    uint32_t unitdata_port;
    
    Remote_entity *mib_info;
    Pdu_header *pdu_header;
    
    uint8_t is_active;

} Client;


typedef struct protocol_state {
    uint32_t packet_size;
    unsigned char *server_port;
    void *server_handle;
    void *server_thread_attributes;
    MIB *mib;
    uint32_t my_cfdp_id;
    Client *newClient;
    Request *current_server_request;
    uint32_t transaction_id;
    uint8_t verbose_level;


} Protocol_state;


void packet_handler_server(Response res, Request *req, Protocol_state *p_state);
void parse_packet_server(unsigned char* buff, uint32_t packet_len, Request *req, Protocol_state *p_state);
void packet_handler_client(Response res, Request *req, Client *client, Protocol_state *p_state);
void user_request_handler(Response res, Request *req, Client *client, Protocol_state *p_state);
void parse_packet_client(unsigned char* buff, Request *req, Client *client, Protocol_state *p_state);


/*  
    unsigned char *source_file_name,
    unsigned char *destination_file_name,
    uint8_t segmentation_control,
    uint8_t fault_handler_overides,
    uint8_t flow_lable,
    uint8_t transmission_mode,
    unsigned char* messages_to_user,
    unsigned char* filestore_requests,
    Client *client,
    Protocol_state *p_state
*/
//returns -1 on error
int put_request(
            unsigned char *source_file_name,
            unsigned char *destination_file_name,
            uint8_t segmentation_control,
            uint8_t fault_handler_overides,
            uint8_t flow_lable,
            uint8_t transmission_mode,
            unsigned char* messages_to_user,
            unsigned char* filestore_requests,
            Client *client,
            Protocol_state *p_state
            );




#endif