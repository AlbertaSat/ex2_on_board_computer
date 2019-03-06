

#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"

static void build_put_packet(char* packet, Request *req, Client* client) {
    
    Pdu_header header;
    header.length_of_entity_IDs = 1;
    header.destination_id = (uint8_t) client->cfdp_id;  
    header.source_id = 2;
    memcpy(packet, &header, 100);


}
//fills the current request with packet data, responses from servers
void parse_packet_client(char* msg, Request *current_request, Client* client, Protocol_state *p_state) {
    ssp_printf("server received %x\n", msg);
}

//fills the current_request struct for the server, incomming requests
void parse_packet_server(char* msg, Request *current_request, Protocol_state *p_state) {
    ssp_printf("server received %x\n", msg);
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

    switch (req->type)
    {
        case put:
                res.msg = req->buff;
                build_put_packet(res.msg, client, req);
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