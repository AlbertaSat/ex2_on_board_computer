

/*------------------------------------------------------------------------------

                                    USER STUFF
                                    aka: request from person

------------------------------------------------------------------------------*/
#include "stdint.h"
#include "protocol_handler.h"
#include "port.h"
#include <string.h>

//Omission of source and destination filenames shall indicate that only Meta
//data will be delivered
int put_request(char *source_file_name,
            char *destination_file_name,
            uint8_t segmentation_control,
            uint8_t fault_handler_overides,
            uint8_t flow_lable,
            uint8_t transmission_mode,
            char* messages_to_user,
            char* filestore_requests,
            Client *client,
            Protocol_state *p_state
            ) {

    uint32_t file_size = get_file_size(source_file_name);
    
    if (file_size == -1)
        return -1;

    //give the client a new request to perform
    Request *req = init_request(client->packet_len);
    req->file = create_file(source_file_name, 0);

    //build a request 
    req->transaction_sequence_number = p_state->transaction_sequence_number++;

    //enumeration
    req->type = put;

    req->dest_cfdp_id = client->cfdp_id;
    req->file_size = file_size;
    
    memcpy(req->source_file_name, source_file_name ,strnlen(source_file_name, MAX_PATH));
    memcpy(req->destination_file_name, destination_file_name, strnlen(destination_file_name, MAX_PATH));

    req->segmentation_control = segmentation_control;
    req->fault_handler_overides = fault_handler_overides;
    req->flow_lable = flow_lable;
    req->transmission_mode = transmission_mode;
    req->messages_to_user = messages_to_user;
    req->filestore_requests = filestore_requests;

    //client->req_queue->insert(client->req_queue, req, p_state->transaction_sequence_number);
    client->req = req;
    return 0;
}
