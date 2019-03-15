

#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"


void build_pdu_header(char* packet, Request *req, Client* client, Protocol_state *p_state) {
    //4 is the header length before id variables
    memcpy(packet, client->pdu_header, PACKET_STATIC_HEADER_LEN);
    //memcpy(packet, "some other stuff\n", 5);
    //memcpy(&packet[PACKET_STATIC_HEADER_LEN], client->pdu_header->destination_id, client->pdu_header->length_of_entity_IDs);
    //memcpy(&packet[PACKET_STATIC_HEADER_LEN + client->pdu_header->length_of_entity_IDs], client->pdu_header->destination_id, client->pdu_header->length_of_entity_IDs);
    

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
    
    ssp_printf("server received %s\n", msg);
    ssp_printf("length of Ids %d:\n", header->length_of_entity_IDs);
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
    build_pdu_header(res.msg, req, client, p_state);

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