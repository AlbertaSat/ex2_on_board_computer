
#include "mib.h"
#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"





//returns the location in the packet where the next pointer for tthe packet will start after the header
static uint8_t build_pdu_header(char *packet, uint64_t transaction_sequence_number, uint32_t transmission_mode, Pdu_header *pdu_header) {
    memcpy(packet, pdu_header, PACKET_STATIC_HEADER_LEN);
    uint32_t packet_index = PACKET_STATIC_HEADER_LEN;
    
    Pdu_header *header = (Pdu_header *)packet;
    header->transmission_mode = transmission_mode;

    //copy variable length src id
    memcpy(&packet[packet_index], 
    pdu_header->source_id, 
    pdu_header->length_of_entity_IDs);
    packet_index += pdu_header->length_of_entity_IDs;

    //copy variable length transaction number id
    memcpy(&packet[packet_index],
    &transaction_sequence_number, 
    pdu_header->transaction_seq_num_len);
    packet_index += pdu_header->transaction_seq_num_len;

    //copy variable length destination id
    memcpy(&packet[packet_index],
    pdu_header->destination_id,
    pdu_header->length_of_entity_IDs);
    
    uint8_t total_bytes = PACKET_STATIC_HEADER_LEN 
    + pdu_header->length_of_entity_IDs 
    + pdu_header->transaction_seq_num_len 
    + pdu_header->length_of_entity_IDs;

    return total_bytes;
}

static uint8_t build_put_packet_metadata(Response res, uint32_t start, Request *req, Client* client, Protocol_state *p_state) {    
    Pdu_header *header = (Pdu_header *) res.msg;
   
    //set header to file directive 0 is directive, 1 is data
    header->PDU_type = 0;
    uint8_t packet_index = start;

    //set directive 1 byte
    Pdu_directive *directive = (Pdu_directive *) &res.msg[packet_index];
    directive->directive_code = META_DATA_PDU;
    packet_index += SIZE_OF_DIRECTIVE_CODE;
    Pdu_meta_data *meta_data_packet = (Pdu_meta_data *) &res.msg[packet_index];

    //1 bytes
    meta_data_packet->segmentation_control = req->segmentation_control;
    meta_data_packet->reserved_bits = 0;
    packet_index++;

    //4 bytes
    uint32_t network_bytes = htonl(req->file_size);
    network_bytes = network_bytes;
    memcpy(&res.msg[packet_index], &network_bytes, sizeof(uint32_t));
    packet_index += 4;
    
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

    //mark the size of the packet
    header->PDU_data_field_len = htons(total_bytes);


    //ssp_print_hex(&res.msg[start], total_bytes);

    return packet_index;
}
static uint8_t build_nak_response(char *packet, uint32_t start, uint32_t offset, Request *req, Client* client, Protocol_state *p_state) {

    if (offset > req->file->total_size) {
        return -1;
    }

    Pdu_header *header = (Pdu_header *) packet;
    //set header to file directive 0 is directive, 1 is data
    header->PDU_type = 1;

    uint16_t packet_index = start;
    File_data_pdu_contents *packet_offset = (File_data_pdu_contents *) &packet[packet_index];
    packet_offset->offset = offset;

    //4 bytes is the size of the offset paramater
    packet_index += 4;
    uint16_t data_size = client->packet_len - packet_index;
    //fill the rest of the packet with data
    int bytes = get_offset(req->file, &packet[packet_index], data_size, offset);
    if (bytes <= 0){
        ssp_error("could not get offset, this could because the file is empty!\n");
        return -1;
    }
    
    //add bytes read, and the packet offset to the data_field length
    header->PDU_data_field_len = htons(bytes + 4);
    if (bytes <  data_size)
        return 1;

}


//requires a req->file to be created
//returns 1 on end of file
static uint8_t build_data_packet(Response res, uint32_t start, Request *req, Client* client, Protocol_state *p_state) {

    if (req->file->next_offset_to_send > req->file->total_size){
        ssp_error("cant send an offset past the file's length\n");
        return -1;
    }

    Pdu_header *header = (Pdu_header *) res.msg;
    //set header to file directive 0 is directive, 1 is data
    header->PDU_type = 1;

    uint16_t packet_index = start;
    File_data_pdu_contents *packet_offset = (File_data_pdu_contents *) &res.msg[packet_index];
    
    //4 bytes is the size of the offset paramater
    packet_offset->offset = req->file->next_offset_to_send;
    packet_index += 4;

    uint16_t data_size = client->packet_len - packet_index;
    
    //fill the rest of the packet with data
    int bytes = get_offset(req->file, &res.msg[packet_index], data_size, req->file->next_offset_to_send);
    if (bytes <= 0){
        ssp_error("could not get offset, this could because the file is empty!\n");
        return -1;
    }

    //calculate checksum for data packet, this is used to calculate in transit checksums
    req->file->partial_checksum += calc_check_sum(&res.msg[packet_index], bytes);
    req->file->next_offset_to_send += bytes;

    //add bytes read, and the packet offset to the data_field length
    header->PDU_data_field_len = htons(bytes + 4);

    if (bytes <  data_size)
        return 1;

    return 0;
}

static void build_eof_packet(Response res, uint32_t start, Request *req, Client* client, Protocol_state *p_state) {

    Pdu_header *header = (Pdu_header *) res.msg;
    //set header to file directive 0 is directive, 1 is data
    header->PDU_type = 0;
    
    uint8_t packet_index = (uint8_t) start;
    Pdu_directive *directive = (Pdu_directive *) &res.msg[packet_index];
    directive->directive_code = EOF_PDU;
    packet_index++;

    Pdu_eof *packet = (Pdu_eof *) &res.msg[packet_index];

    //this will be need to set from the req struct probably.
    //4 bits, 
    packet->condition_code = COND_NO_ERROR;
    //4 bits reserved bits
    packet->spare = 0;
    packet_index++;

    //4 bytes
    packet->file_size = ntohl(req->file->total_size);
    packet_index += 4;

    //TODO checksum procedures
    packet->checksum = req->file->partial_checksum;
    packet_index += 4;

    //TODO addTLV fault_location
    header->PDU_data_field_len = htons(packet_index - start);

}


void print(void *element, void *args) {
    Offset * o = (Offset *) element;
    ssp_printf("start: %u, end: %u\n", o->start, o->end);
}



struct offset_holder {
    Offset *offsets;
    int i;
};


static void fill_nak_array(void *element, void *args){
    struct offset_holder *holder = (struct offset_holder *)args;
    
    Offset *offset = (Offset *)element;

    holder->offsets[holder->i].start = htonl(offset->start);
    holder->offsets[holder->i].end = htonl(offset->end);

    ssp_printf("offset: start:%u, end:%u\n", offset->start, offset->end);
    holder->i++;
}

void build_nak_packet(char *packet, uint32_t start, Request *req) {
    
    packet[start] = NAK_PDU;
    uint32_t packet_index = start + 1;
    Pdu_nak *pdu_nak = (Pdu_nak *) &packet[packet_index];
    uint64_t count = req->file->missing_offsets->count;
    
    //fill offset array with all missing offsets [holder.offsets]
    struct offset_holder holder;
    holder.offsets = ssp_alloc(count, sizeof(Offset));
    holder.i = 0;

    req->file->missing_offsets->print(req->file->missing_offsets, fill_nak_array, &holder);
    
    pdu_nak->start_scope = holder.offsets[0].start;;
    pdu_nak->end_scope = holder.offsets[holder.i-1].end;;
    pdu_nak->segment_requests = htonll(count);
    packet_index+=16;
   
    memcpy(&packet[packet_index], holder.offsets, sizeof(Offset) * count);
    ssp_free(holder.offsets);
}

static void process_pdu_eof(char *packet, Request *req) {

    Pdu_eof *eof_packet = (Pdu_eof *) packet;
    if (eof_packet->checksum == req->file->partial_checksum) {

        //ssp_printf("received checksum: %u\n", req->file->partial_checksum);
        //ssp_printf("actual checksum: %u\n", eof_packet->checksum);
        uint32_t file_size = ntohl(eof_packet->file_size);

        ssp_printf("received eof packet, filesize: %u\n", file_size);
        
        //ssp_printf("missing offsets after file received eof: count: %u\n", req->file->missing_offsets->count);
        req->file->missing_offsets->print(req->file->missing_offsets, print, NULL);

        req->file->eof_checksum = eof_packet->checksum;

    
        //if transmission mode unacknowledged, close file 
        if (req->transmission_mode == 0)
            return;

        if (ssp_close(req->file->fd) == -1)
            ssp_error("could not close file\n");

    }
}

//TODO This needs more work, file handling when files already exist ect
static int process_file_request_metadata(Request *req) {

    if (does_file_exist(req->destination_file_name)){
        //ssp_error("file already exists, overwriting it\n");
        req->file = create_file(req->destination_file_name, 1);
    }
    if (req->file == NULL) {
        //ssp_printf("creating file: %s\n", req->destination_file_name);
        req->file = create_file(req->destination_file_name, 1);
    }

    ssp_printf("received metadata packet\n");
    //add offset for naks
    Offset *offset = ssp_alloc(1, sizeof(Offset));
    offset->end = req->file_size;
    offset->start = 0;
    
    //transmission mode 1 is unacknowledged, therefor no offsets
    if (req->transmission_mode == 1)
        return 1;

    ssp_printf("mode acknowledged, building offset map\n");
    req->file->missing_offsets->insert(req->file->missing_offsets, offset, req->file_size);

    return 1;
}

static int process_pdu_header(char*packet, Request *req, Protocol_state *p_state) {

    uint8_t packet_index = PACKET_STATIC_HEADER_LEN;
    Pdu_header *header = (Pdu_header *) packet;

    uint32_t source_id = 0;
    memcpy(&source_id, &packet[packet_index], header->length_of_entity_IDs);
    packet_index += header->length_of_entity_IDs;

    //TODO the transaction number should get the request from data structure hosting requests
    uint32_t transaction_sequence_number = 0;
    memcpy(&transaction_sequence_number, &packet[packet_index], header->transaction_seq_num_len);
    packet_index += header->transaction_seq_num_len;

    uint32_t dest_id = 0;
    memcpy(&dest_id, &packet[packet_index], header->length_of_entity_IDs);
    packet_index += header->length_of_entity_IDs;

    if (p_state->verbose_level == 3) {
        ssp_printf("------------printing_header_received------------\n");
        ssp_print_hex(packet, packet_index);
        ssp_printf("packet data length %d, sequence number %d\n", transaction_sequence_number);
    }

    if (p_state->my_cfdp_id != dest_id){
        ssp_printf("someone is sending packets here that are not for me\n");
        return -1;
    }

    req->transmission_mode = header->transmission_mode;
    req->dest_cfdp_id = source_id;
    req->transaction_sequence_number = transaction_sequence_number;
    
    return packet_index;

}

static void write_packet_data_to_file(char *data_packet, uint32_t data_len,  File *file) {
    if(file == NULL)
        ssp_error("file struct is null, can't write to file");


    File_data_pdu_contents *packet = (File_data_pdu_contents *)data_packet;
    uint32_t offset_start = packet->offset;
    uint32_t offset_end = offset_start + data_len - 4;

    //ssp_printf("packet offset received: %d\n", packet->offset);
    
    int bytes = write_offset(file, &data_packet[4], data_len - 4, offset_start);
    if (bytes <= 0) {
        ssp_error("no new data was written\n");
        return;
    }

    file->partial_checksum += calc_check_sum(&data_packet[4], bytes);
    
    if (file->missing_offsets->count == 0)
        return;

    receive_offset(file, 0, offset_start, offset_end);
}


static void fill_request_pdu_metadata(char *meta_data_packet, Request *req_to_fill) {

    Pdu_meta_data *meta_data = (Pdu_meta_data *) meta_data_packet;
    req_to_fill->segmentation_control = meta_data->segmentation_control;

    uint8_t packet_index = 1;
    uint32_t *network_bytes = (uint32_t*) &meta_data_packet[packet_index];
    uint32_t file_size = ntohl(*network_bytes);

    req_to_fill->file_size = file_size;
    packet_index += 4;

    uint8_t file_name_len = meta_data_packet[packet_index];
    packet_index++;

    memcpy(req_to_fill->source_file_name, &meta_data_packet[packet_index], file_name_len);
    packet_index += file_name_len;

    file_name_len = meta_data_packet[packet_index];
    packet_index++;
    memcpy(req_to_fill->destination_file_name, &meta_data_packet[packet_index], file_name_len);

    packet_index += file_name_len;
    //ssp_print_hex(meta_data_packet, packet_index);

    return;
}


/*------------------------------------------------------------------------------

                                    REMOTE SIDE
                                    aka: handles responses from server

------------------------------------------------------------------------------*/



//fills the current request with packet data, responses from servers
void parse_packet_client(char *packet, Response res, Request *req, Client* client, Protocol_state *p_state) {
 
    
    uint32_t packet_index = process_pdu_header(packet, req, p_state);
    Pdu_header *header = (Pdu_header *) packet;    
    uint16_t packet_data_len = ntohs(header->PDU_data_field_len);
    if (packet[packet_index] == NAK_PDU){
        ssp_printf("client received nak\n");
        packet_index += 1; 
        Pdu_nak *nak = (Pdu_nak *) &packet[packet_index];
        uint32_t ostart = ntohl(nak->start_scope);
        uint32_t oend = ntohl(nak->end_scope);
        uint64_t segments = ntohll(nak->segment_requests);

        if (req->buff == NULL){
            ssp_printf("req->buff is null\n");
        }
        if (client->pdu_header == NULL){
            ssp_printf("pdu_header is null\n");
        }

        uint32_t outgoing_packet_index = build_pdu_header(req->buff, req->transaction_sequence_number, 0, client->pdu_header);
        
        
        //for (int i = 0; i < segments; i++){
            build_nak_response(req->buff, outgoing_packet_index, ostart, req, client, p_state) ;
            ssp_sendto(res);
        //}
        

        
        //ssp_printf("req sequence nubmer: %u\n", req->transaction_sequence_number);
        //uint32_t outgoing_packet_index = build_pdu_header(req->buff, client->outGoing_req->transaction_sequence_number, 0, client->outGoing_req->pdu_header);        
        //ssp_sendto(client->outGoing_req->res);
         //ssp_printf("client received start: %u, end: %u\n", ostart, oend);
        //ssp_print_hex(packet, 40);

    }
}


/*------------------------------------------------------------------------------

                                    SERVER SIDE
                                    aka: handles responses from remote

------------------------------------------------------------------------------*/

void on_server_time_out(Response res, Request *req, Protocol_state *p_state) {
    //ssp_printf("time out missing packet count:%d\n", req->file->missing_offsets->count);

    res.msg = req->buff;
    if (req->file->missing_offsets->count > 0) {
        memset(res.msg, 0, res.packet_len);
        uint8_t start = build_pdu_header(res.msg, req->transaction_sequence_number, 1, req->pdu_header);
        build_nak_packet(res.msg, start, req);
        ssp_sendto(res);
        return;
    }    
}

//Server responses
void packet_handler_server(Response res, Request *current_request, Protocol_state *p_state) {
    //res.msg = "Server Received\n";
    //ssp_sendto(res);
}


//fills the current_request struct for the server, incomming requests
void parse_packet_server(char *packet, uint32_t packet_len, Request *req, Protocol_state *p_state) {


    uint8_t packet_index = PACKET_STATIC_HEADER_LEN;
    Pdu_header *header = (Pdu_header *) packet;

    //most of this can be replaced with the process header func
    uint32_t source_id = 0;
    memcpy(&source_id, &packet[packet_index], header->length_of_entity_IDs);
    packet_index += header->length_of_entity_IDs;

    //TODO the transaction number should get the request from data structure hosting requests
    uint32_t transaction_sequence_number = 0;
    memcpy(&transaction_sequence_number, &packet[packet_index], header->transaction_seq_num_len);
    packet_index += header->transaction_seq_num_len;

    uint32_t dest_id = 0;
    memcpy(&dest_id, &packet[packet_index], header->length_of_entity_IDs);
    packet_index += header->length_of_entity_IDs;

    uint16_t packet_data_len = ntohs(header->PDU_data_field_len);

    if (p_state->verbose_level == 3) {
        ssp_printf("------------printing_header_received------------\n");
        ssp_print_hex(packet, packet_index);
        ssp_printf("packet data length %d, sequence number %d\n", packet_data_len, transaction_sequence_number);
    }

    if (p_state->my_cfdp_id != dest_id){
        ssp_printf("someone is sending packets here that are not for me\n");
        return;
    }

    req->transmission_mode = header->transmission_mode;
    req->dest_cfdp_id = source_id;
    req->transaction_sequence_number = transaction_sequence_number;

    //process file data
    if (header->PDU_type == 1) {

        write_packet_data_to_file(&packet[packet_index], packet_data_len, req->file);
        return;
    }
    
    //set header for responding later
    if (req->pdu_header == NULL){
        req->pdu_header = get_header_from_mib(p_state->mib, source_id);
    }

    Pdu_directive *directive = (Pdu_directive *) &packet[packet_index];
    packet_index++;

    switch (directive->directive_code)
    {
        case META_DATA_PDU:
            fill_request_pdu_metadata(&packet[packet_index], req);
            process_file_request_metadata(req);
            break;
    
        case EOF_PDU:
            process_pdu_eof(&packet[packet_index], req);
            
            
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

    uint32_t start = build_pdu_header(res.msg, req->transaction_sequence_number, req->transmission_mode, client->pdu_header);

    switch (req->type)
    {
        case eof: 
            build_eof_packet(res, start, req, client, p_state);
            ssp_sendto(res);
            req->type = none;
            break;

        case sending_data: 
            if (build_data_packet(res, start, req, client, p_state))
                req->type = eof;
            
            if (p_state->verbose_level == 3) {
                ssp_printf("------------sending_a_data_packets-----------\n");
                ssp_print_hex(res.msg, start);
            }
            ssp_sendto(res);
            break;

        case put:
            start = build_put_packet_metadata(res, start, req, client, p_state);
            
            if (p_state->verbose_level == 3) {
                ssp_printf("------------sending_a_put_request------------\n");
                ssp_print_hex(res.msg, start);
            }

            ssp_sendto(res);
            req->type = sending_data;
            break;


        default:
            break;
    }

}







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
    Request *req = client->outGoing_req;
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
    return 0;
}


