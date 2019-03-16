

#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"


static void build_pdu_header(char* packet, Client* client, Protocol_state *p_state) {
    memcpy(packet, client->pdu_header, PACKET_STATIC_HEADER_LEN);
    
    //copy variable length src id
    memcpy(&packet[PACKET_STATIC_HEADER_LEN], 
    client->pdu_header->source_id, 
    client->pdu_header->length_of_entity_IDs);
    
    //copy variable length transaction number id
    memcpy(&packet[PACKET_STATIC_HEADER_LEN + client->pdu_header->length_of_entity_IDs],
    &client->pdu_header->transaction_sequence_number, 
    client->pdu_header->transaction_seq_num_len);
    
    //copy variable length destination id
    memcpy(&packet[PACKET_STATIC_HEADER_LEN + client->pdu_header->length_of_entity_IDs + client->pdu_header->transaction_seq_num_len],
    client->pdu_header->destination_id,
    client->pdu_header->length_of_entity_IDs);    
}

static void build_put_packet(char* packet, Request *req, Client* client, Protocol_state *p_state) {    
}

//fills the current request with packet data, responses from servers
void parse_packet_client(char* msg, Request *current_request, Client* client, Protocol_state *p_state) {
    ssp_printf("client received %x\n", msg);
}

//fills the current_request struct for the server, incomming requests
void parse_packet_server(char* msg, Request *current_request, Protocol_state *p_state) {
    Pdu_header *header = (Pdu_header *) msg;

    uint32_t source_id = 0;
    memcpy(&source_id, &msg[PACKET_STATIC_HEADER_LEN], 
    header->length_of_entity_IDs);

    uint32_t transaction_sequence_number = 0;
    memcpy(&transaction_sequence_number, &msg[PACKET_STATIC_HEADER_LEN + header->length_of_entity_IDs],
    header->transaction_seq_num_len);

    uint32_t dest_id = 0;
    memcpy(&dest_id, &msg[PACKET_STATIC_HEADER_LEN + header->length_of_entity_IDs + header->transaction_seq_num_len],
    header->length_of_entity_IDs);

    if (p_state->my_cfdp_id != dest_id){
        ssp_printf("someone is sending packets here that are not for me\n");
        return;
    }

    






    ssp_printf("server received %s\n", msg);
    ssp_printf("src id %u\n",source_id); 
    ssp_printf("dest id %d\n", dest_id);
    ssp_printf("transaction sequence number %d\n", transaction_sequence_number);
}

//Server responses
void packet_handler_server(Response res, Request *current_request, Protocol_state *p_state) {
    //res.msg = "Server Received\n";
    //ssp_sendto(res);
}

//Client responses
void packet_handler_client(Response res, Request *req, Client* client, Protocol_state *p_state) {
    //res.msg = "Client Received!\n";
    //ssp_sendto(res);
}  

//current user request, to send to client
void user_request_handler(Response res, Request *req, Client* client, Protocol_state *p_state) {

    res.msg = req->buff;
    build_pdu_header(res.msg, client, p_state);

    switch (req->type)
    {
        case put:
                build_put_packet(res.msg, req, client, p_state);
                ssp_sendto(res);
            break;
    
        default:
            break;
    }

}

//Omission of source and destination filenames shall indicate that only Meta
//data will be delivered

void put_request(char *dest_cfdp_id, 
            char *source_file_name,
            char *destination_file_name,
            uint8_t segmentation_control,
            uint8_t fault_handler_overides,
            uint8_t flow_lable,
            uint8_t transmission_mode,
            char* messages_to_user,
            char* filestore_requests
            ) {

    Request *req = ssp_alloc(1, sizeof(Request));
    if (checkAlloc(req, 0)) {
        ssp_error("cound't allocate mem for request\n");
    }
}

/*
(destination CFDP entity ID,
[source file name],
[destination file name],
[segmentation control],
[fault handler overrides],
[flow label],
[transmission mode],
[messages to user],
[filestore requests]) 
*/