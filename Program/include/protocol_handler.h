
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "list.h"
#include "packet.h"
#include "mib.h"
#include "filesystem_funcs.h"
#include "requests.h"

typedef struct protocol_state {
    uint32_t packet_size;
    char *server_port;
    void *server_handle;
    MIB *mib;
    uint32_t my_cfdp_id;
    
    List* request_list; 

    Request *current_request;
   
    //lock this
    uint32_t transaction_sequence_number;
    
    uint8_t verbose_level;

    //bool for exiting the server thread
    uint8_t close;

} Protocol_state;


//outgoing requests spin up client threads
typedef struct client {
    
    void *client_handle;
    uint32_t packet_len;
    
    Request *current_request;
    List *request_list;

    //can get rid of these, these are found in mib_info anyway
    uint32_t cfdp_id;
    uint32_t unitdata_id;
    uint32_t unitdata_port;
    
    Remote_entity *mib_info;
    Pdu_header *pdu_header;
    
    Protocol_state *p_state;    

    //bool for exiting the client thread
    uint8_t close;


} Client;


//for use
void parse_packet_server(char* buff, uint32_t packet_index, Response res, Request *req, Protocol_state *p_state);
void user_request_handler(Response res, Request *req, Client *client);
void parse_packet_client(char* buff, uint32_t packet_index, Response res, Request *req, Client *client);
void on_server_time_out(Response res, Request *current_request);
int process_pdu_header(char*packet, Response res, Request **req, List *request_list, Protocol_state *p_state);
uint8_t build_data_packet(char *packet, uint32_t start, File *file, uint32_t length);

//for testing
uint8_t build_pdu_header(char *packet, uint64_t transaction_sequence_number, uint32_t transmission_mode, Pdu_header *pdu_header);
uint8_t build_ack (char *packet, uint32_t start, uint8_t type);
int process_file_request_metadata(Request *req);
int nak_response(char *packet, uint32_t start, Request *req, Response res, Client *client);
void set_data_length(char*packet, uint16_t data_len);
uint32_t build_nak_packet(char *packet, uint32_t start, Request *req);
uint16_t get_data_length(char*packet);

Request *put_request(
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