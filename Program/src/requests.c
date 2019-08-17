

/*------------------------------------------------------------------------------

                                    USER STUFF
                                    aka: request from person

------------------------------------------------------------------------------*/
#include "stdint.h"
#include "protocol_handler.h"
#include "port.h"
#include "utils.h"
#include <string.h>
#include "tasks.h"
#include "types.h"
#include "filesystem_funcs.h"
#include "mib.h"

void ssp_cleanup_req(void *request) {

    if (request == NULL)
        return;

    Request *req = (Request *) request;

    if (req->file != NULL)
        free_file(req->file);
    if (req->pdu_header != NULL)
        ssp_cleanup_pdu_header(req->pdu_header);
    if (req->source_file_name != NULL)  
        ssp_free(req->source_file_name);
    if (req->destination_file_name != NULL)
        ssp_free(req->destination_file_name);
    if (req->buff != NULL)
        ssp_free(req->buff);
    if (req->res.addr != NULL)
        ssp_free(req->res.addr);
    if (req != NULL)
        ssp_free(req);

}


Request *init_request(uint32_t buff_len) {

    Request *req = ssp_alloc(1, sizeof(Request));

    req->source_file_name = ssp_alloc(MAX_PATH, sizeof(char));
    checkAlloc(req->source_file_name, 1);

    req->destination_file_name = ssp_alloc(MAX_PATH, sizeof(char));
    checkAlloc(req->destination_file_name,  1);

    req->file = NULL;
    req->buff_len = buff_len;
    req->buff = ssp_alloc(buff_len, sizeof(char));
    req->type = none;
    checkAlloc(req->buff,  1);
    return req;
}

//Omission of source and destination filenames shall indicate that only Meta
//data will be delivered
Request *put_request(char *source_file_name,
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
        return NULL;

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

    req->res.addr = ssp_alloc(sizeof(uint64_t), 1);

    client->request_list->insert(client->request_list, req, 0);

    return req;
}
