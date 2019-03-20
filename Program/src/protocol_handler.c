

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
   
    //set header to file directive
    header->PDU_type = 1;
    
    uint8_t packet_index = start;

    //set directive 1 byte
    Pdu_directive *directive = &res.msg[packet_index];
    directive->directive_code = META_DATA_PDU;
    packet_index += SIZE_OF_DIRECTIVE_CODE;
    Pdu_meta_data *meta_data_packet = &res.msg[packet_index];

    //1 bytes
    meta_data_packet->segmentation_control = 0;
    meta_data_packet->reserved_bits = 0;
    
    //4 bytes
    meta_data_packet->file_size = 1;
    packet_index += 5;

    //variable length params
    uint8_t src_file_name_length = 5;
    uint8_t destination_file_length = 7;
    char *src_file_name = "hero";
    char *destination_file_name = "thereo";
    
    
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


    ssp_printf("------------printing_metadata_contents-----------\n");
    ssp_print_hex(&res.msg[start], total_bytes);

    return packet_index;
}




//fills the current request with packet data, responses from servers
void parse_packet_client(unsigned char *msg, Request *current_request, Client* client, Protocol_state *p_state) {
    ssp_printf("client received %x\n", msg);
}

//fills the current_request struct for the server, incomming requests
void parse_packet_server(unsigned char *msg, uint32_t packet_len, Request *current_request, Protocol_state *p_state) {
    Pdu_header *header = (Pdu_header *) msg;

    uint32_t source_id = 0;
    memcpy(&source_id, &msg[PACKET_STATIC_HEADER_LEN], 
    header->length_of_entity_IDs);

    uint32_t transaction_sequence_number = 0;
    memcpy(&transaction_sequence_number, &msg[PACKET_STATIC_HEADER_LEN 
    + header->length_of_entity_IDs],
    header->transaction_seq_num_len);

    uint32_t dest_id = 0;
    memcpy(&dest_id, &msg[PACKET_STATIC_HEADER_LEN 
    + header->length_of_entity_IDs 
    + header->transaction_seq_num_len],
    header->length_of_entity_IDs);

    ssp_printf("server received: ");
   

    if (p_state->my_cfdp_id != dest_id){
        ssp_printf("someone is sending packets here that are not for me\n");
        return;
    }

    //process file directive
    if (header->PDU_type == 0) {
        
    }

    
    //ssp_printf("src id %u\n",source_id);
    //ssp_printf("dest id %d\n", dest_id);
    //ssp_printf("transaction sequence number %d\n", transaction_sequence_number);

    memset(msg, 0, packet_len);
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
    uint32_t start = build_pdu_header(res, req, client, p_state);
    
    ssp_printf("\n..................printing_header..................\n");
    ssp_print_hex(res.msg, start);

    switch (req->type)
    {
        case put:
                build_put_packet_metadata(res, start, req, client, p_state);
                ssp_sendto(res);
            break;
    
        default:
            break;
    }

}

//Omission of source and destination filenames shall indicate that only Meta
//data will be delivered

void put_request(char *dest_cfdp_id, 
            unsigned char *source_file_name,
            unsigned char *destination_file_name,
            uint8_t segmentation_control,
            uint8_t fault_handler_overides,
            uint8_t flow_lable,
            uint8_t transmission_mode,
            unsigned char* messages_to_user,
            unsigned char* filestore_requests
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