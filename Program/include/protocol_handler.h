
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "utils.h"
#include "packet.h"
#include "mib.h"
#include "filesystem_funcs.h"

typedef struct response {
    int sfd;
    //this is a pointer to buff in a request
    char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    //getting rid of this soon in favour of client based packet sizes
    uint32_t packet_len;
    
} Response;

typedef enum Indication {
    finished, 
    none,
    transaction,
    put,
    eof,
    //sending data
    sending_data

} Indication;

//incoming requests spin up requests
typedef struct request {
    Indication type;
    uint64_t transaction_sequence_number;
    uint32_t dest_cfdp_id;

    File *file;
    uint32_t file_size;

    char *source_file_name;
    char *destination_file_name;

    uint32_t packet_data_len;

    uint8_t segmentation_control;
    uint8_t fault_handler_overides;
    uint8_t flow_lable;
    uint8_t transmission_mode;

    uint8_t received_metadata;
    uint8_t received_eof;
    //counter for resending eof packets == 3
    uint8_t resent_eof;
    uint8_t received_finished;
    //counter for resending finished packets == 3
    uint8_t resent_finished;
    //bool for sending first blast of data packets
    uint8_t sent_first_data_round;

    char* messages_to_user;
    char* filestore_requests;
    
    //sets the buffer length, for making packets
    uint32_t buff_len;

    //buffer for making packets
    char* buff;

    //header for building response packets
    Pdu_header *pdu_header;

    //handler for sending responses back
    Response res;
} Request;


typedef struct protocol_state {
    uint32_t packet_size;
    char *server_port;
    void *server_handle;
    void *server_thread_attributes;
    MIB *mib;
    uint32_t my_cfdp_id;
    
    List* request_list; 

    Request *current_server_request;
   
    //lock this
    uint32_t transaction_sequence_number;
    
    uint8_t verbose_level;


} Protocol_state;


//outgoing requests spin up client threads
typedef struct client {
    
    void *client_handle;
    void *client_thread_attributes;
    uint32_t packet_len;
    
    Request *req;
    List *req_queue;

    //can get rid of these, these are found in mib_info anyway
    uint32_t cfdp_id;
    uint32_t unitdata_id;
    uint32_t unitdata_port;
    
    Remote_entity *mib_info;
    Pdu_header *pdu_header;
    
    uint8_t is_active;
    Protocol_state *p_state;
    
} Client;


//for use
void parse_packet_server(char* buff, uint32_t packet_len, Response res, Request *req, Protocol_state *p_state);
void user_request_handler(Response res, Request *req, Client *client);
void parse_packet_client(char* buff, Response res, Request *req, Client *client);
void on_server_time_out(Response res, Request *current_request, Protocol_state *p_state);


//for testing
uint8_t build_pdu_header(char *packet, uint64_t transaction_sequence_number, uint32_t transmission_mode, Pdu_header *pdu_header);
uint8_t build_ack (char *packet, uint32_t start, uint8_t type, Request *req);
int process_file_request_metadata(Request *req);
int nak_response(char *packet, uint32_t start, Request *req, Response res, Client *client);
void set_data_length(char*packet, uint16_t data_len);
uint32_t build_nak_packet(char *packet, uint32_t start, Request *req);
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
            char *source_file_name,
            char *destination_file_name,
            uint8_t segmentation_control,
            uint8_t fault_handler_overides,
            uint8_t flow_lable,
            uint8_t transmission_mode,
            char* messages_to_user,
            char* filestore_requests,
            Client *client,
            Protocol_state *p_state
            );




#endif