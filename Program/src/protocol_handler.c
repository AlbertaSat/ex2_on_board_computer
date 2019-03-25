

#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"





//returns the location in the packet where the next pointer for tthe packet will start after the header
static uint8_t build_pdu_header(Response res, Request *req, Client* client, Protocol_state *p_state) {
    unsigned char *packet = res.msg;
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
    
    uint8_t total_bytes = PACKET_STATIC_HEADER_LEN 
    + client->pdu_header->length_of_entity_IDs 
    + client->pdu_header->transaction_seq_num_len 
    + client->pdu_header->length_of_entity_IDs;

    return total_bytes;
}

static uint8_t build_put_packet_metadata(Response res, uint32_t start, Request *req, Client* client, Protocol_state *p_state) {    
    Pdu_header *header = (Pdu_header *) res.msg;
   
    //set header to file directive 0 is directive, 1 is data
    header->PDU_type = 0;
    
    uint8_t packet_index = start;

    //set directive 1 byte
    Pdu_directive *directive = &res.msg[packet_index];
    directive->directive_code = META_DATA_PDU;
    packet_index += SIZE_OF_DIRECTIVE_CODE;
    Pdu_meta_data *meta_data_packet = &res.msg[packet_index];

    //1 bytes
    meta_data_packet->segmentation_control = req->segmentation_control;
    meta_data_packet->reserved_bits = 0;
    
    //4 bytes
    meta_data_packet->file_size = req->file_size;
    packet_index += 5;

    //variable length params
    uint8_t src_file_name_length = strnlen(req->source_file_name, MAX_PATH);
    uint8_t destination_file_length = strnlen(req->destination_file_name, MAX_PATH);
    char *src_file_name = req->source_file_name;
    char *destination_file_name = req->destination_file_name;
    
    
    //copy source length to packet (1 bytes) 
    memcpy(&res.msg[packet_index], &src_file_name_length, src_file_name_length);
    packet_index++;
    //copy source name to packet (length bytes) 
    memcpy(&res.msg[packet_index], src_file_name, src_file_name_length);
    packet_index += src_file_name_length;


    //copy length to packet (1 byte)
    memcpy(&res.msg[packet_index], &destination_file_length, 1);
    packet_index++;
    
    //copy destination name to packet (length bytes)
    memcpy(&res.msg[packet_index], destination_file_name, destination_file_length);
    packet_index += destination_file_length;
    uint8_t total_bytes = packet_index - start; 
    return packet_index;
}

//requires a req->file to be created
//returns 1 on end of file
static uint8_t build_data_packet(Response res, uint32_t start, Request *req, Client* client, Protocol_state *p_state) {
    
    uint32_t packet_index = start;
    File_data_pdu_contents *packet_offset = &res.msg[packet_index];

    //4 bytes
    packet_offset->offset = 1;
    packet_index += 4;

    uint32_t data_size = client->packet_len - packet_index;
    
    //fill the rest of the packet with data
    int bytes = get_offset(req->file, &res.msg[packet_index], data_size, 0);

    if (bytes <  data_size)
        return 1;
    return 0;
}

//TODO write file data
static void process_file_request(Request *req) {

    //check if file exists, if it doesn't create it
    //printf("%s\n", req->destination_file_name);
    //printf("%d\n", get_file_size(req->destination_file_name));

    if (does_file_exist(req->destination_file_name))
        return;

    if (req->file == NULL)
        req->file = create_file(req->destination_file_name);
}


static void fill_request(unsigned char *packet, Request *req_to_fill) {

    Pdu_meta_data *meta_data = packet;
    req_to_fill->segmentation_control = meta_data->segmentation_control;

    uint8_t packet_index = 4;

    uint32_t file_size = (uint32_t)packet[packet_index];
    req_to_fill->file_size = file_size;
    packet_index++;

    uint8_t file_name_len = packet[packet_index];
    packet_index++;
    
    memcpy(req_to_fill->source_file_name, &packet[packet_index], file_name_len);
    packet_index += file_name_len + 1;

    file_name_len = packet[packet_index];
    memcpy(req_to_fill->destination_file_name, &packet[packet_index], file_name_len);
    packet_index += file_name_len;

    return;
}


/*------------------------------------------------------------------------------

                                    REMOTE SIDE
                                    aka: handles responses from server

------------------------------------------------------------------------------*/



//fills the current request with packet data, responses from servers
void parse_packet_client(unsigned char *msg, Request *current_request, Client* client, Protocol_state *p_state) {
    ssp_printf("client received %x\n", msg);
}


//Client responses
void packet_handler_client(Response res, Request *req, Client* client, Protocol_state *p_state) {
    //res.msg = "Client Received!\n";
    //ssp_sendto(res);
}  


/*------------------------------------------------------------------------------

                                    SERVER SIDE
                                    aka: handles responses from remote

------------------------------------------------------------------------------*/

//Server responses
void packet_handler_server(Response res, Request *current_request, Protocol_state *p_state) {
    //res.msg = "Server Received\n";
    //ssp_sendto(res);
}


//fills the current_request struct for the server, incomming requests
void parse_packet_server(unsigned char *packet, uint32_t packet_len, Request *current_request, Protocol_state *p_state) {

    uint8_t packet_index = PACKET_STATIC_HEADER_LEN;
    Pdu_header *header = (Pdu_header *) packet;

    uint32_t source_id = 0;
    memcpy(&source_id, &packet[packet_index], header->length_of_entity_IDs);
    packet_index += header->length_of_entity_IDs;

    uint32_t transaction_sequence_number = 0;
    memcpy(&transaction_sequence_number, &packet[packet_index], header->transaction_seq_num_len);
    packet_index += header->transaction_seq_num_len;

    uint32_t dest_id = 0;
    memcpy(&dest_id, &packet[packet_index], header->length_of_entity_IDs);
    packet_index += header->length_of_entity_IDs;


    if (p_state->verbose_level == 3) {
        ssp_printf("------------printing_header_received------------\n");
        ssp_print_hex(packet, packet_index);
    }

    if (p_state->my_cfdp_id != dest_id){
        ssp_printf("someone is sending packets here that are not for me\n");
        return;
    }

    //process file data
    if (header->PDU_type == 1) {
        if (p_state->verbose_level == 3)
            ssp_printf("receiving data\n");

        return;
    }

    current_request->dest_cfdp_id = source_id;
    Pdu_directive *directive = &packet[packet_index];
    packet_index++;

    switch (directive->directive_code)
    {
        case META_DATA_PDU:
            fill_request(&packet[packet_index], current_request);
            process_file_request(current_request);

            break;
    
        default:
            break;
    }

    memset(packet, 0, packet_len);
}




/*------------------------------------------------------------------------------

                                    USER STUFF
                                    aka: request from person

------------------------------------------------------------------------------*/


//current user request, to send to remote
void user_request_handler(Response res, Request *req, Client* client, Protocol_state *p_state) {

    if (req->type == none)
        return;

    res.msg = req->buff;
    memset(res.msg, 0, client->packet_len);

    uint32_t start = build_pdu_header(res, req, client, p_state);

    switch (req->type)
    {
        case put:
                start = build_put_packet_metadata(res, start, req, client, p_state);
                
                if (p_state->verbose_level == 3) {
                    ssp_printf("------------sending_a_put_request------------\n");
                    ssp_print_hex(res.msg, start);
                }

                ssp_sendto(res);
                req->type = sending_data;

            break;
        case sending_data: 
            
                if (build_data_packet(res, start, req, client, p_state) && p_state->verbose_level == 3)
                    ssp_printf("reached end of file\n");

                if (p_state->verbose_level == 3) {
                    ssp_printf("------------sending_a_data_packets-----------\n");
                    ssp_print_hex(res.msg, start);
                }
                ssp_sendto(res);

        default:
            break;
    }

}







//Omission of source and destination filenames shall indicate that only Meta
//data will be delivered
int put_request(unsigned char *source_file_name,
            unsigned char *destination_file_name,
            uint8_t segmentation_control,
            uint8_t fault_handler_overides,
            uint8_t flow_lable,
            uint8_t transmission_mode,
            unsigned char* messages_to_user,
            unsigned char* filestore_requests,
            Client *client,
            Protocol_state *p_state
            ) {

    uint32_t file_size = get_file_size(source_file_name);
    
    if (file_size == -1)
        return -1;

    //give the client a new request to perform
    Request *req = client->outGoing_req;
    req->file = create_file(source_file_name);
    //build a request
    req->transaction_id = p_state->transaction_id++;
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
}


