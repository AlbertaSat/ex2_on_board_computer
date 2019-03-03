

#include "port.h"
#include "protocol_handler.h"


void parse_packet(char* msg, Request req, Protocol_state *p_state) {

}


void packet_handler_server(Response res, Request req, Protocol_state *p_state) {
    res.msg = "Server Received\n";
    ssp_sendto(res);
}


void packet_handler_client(Response res, Request req, Protocol_state *p_state) {
    res.msg = "Client Received!\n";
    ssp_sendto(res);
}  

//Omission of source and destination filenames shall indicate that only Meta
//data will be delivered

put_request(char *dest_cfdp_id, 
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