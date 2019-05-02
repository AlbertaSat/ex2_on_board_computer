
#include "mib.h"
#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"

//returns the location in the packet where the next pointer for tthe packet will start after the header
uint8_t build_pdu_header(char *packet, uint64_t transaction_sequence_number, uint32_t transmission_mode, Pdu_header *pdu_header) {

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

static uint8_t build_finished_pdu(char *packet, uint32_t start, Request *req) {

    uint32_t packet_index = start;
    uint32_t data_len = 0;

    Pdu_directive *directive = (Pdu_directive *) &packet[packet_index];
    directive->directive_code = FINISHED_PDU;
    packet_index++;
    data_len++;

    //one byte
    Pdu_finished *pdu_finished = (Pdu_finished *) &packet[packet_index];
    pdu_finished->condition_code = COND_NO_ERROR;
    pdu_finished->delivery_code = 0;
    pdu_finished->file_status = FILE_RETAINED_SUCCESSFULLY;
    data_len += 1;
    packet_index += 1;
    
    return data_len;
}


static uint8_t build_put_packet_metadata(Response res, uint32_t start, Request *req, Client* client, Protocol_state *p_state) {    
    Pdu_header *header = (Pdu_header *) res.msg;
   
    header->PDU_type = DIRECTIVE;
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

    return packet_index;
}
static uint8_t build_nak_response(char *packet, uint32_t start, uint32_t offset, Request *req, Client* client) {

    if (offset > req->file->total_size) {
        return -1;
    }

    Pdu_header *header = (Pdu_header *) packet;
    header->PDU_type = DATA;

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
    packet->checksum = req->file->partial_checksum;
    packet_index += 4;

    //TODO addTLV fault_location
    header->PDU_data_field_len = htons(packet_index - start);

}

void print(void *element, void *args) {
    Offset * o = (Offset *) element;
    ssp_printf("start: %u, end: %u\n", o->start, o->end);
}


//this is a callback for building nak_array server side
struct offset_holder {
    Offset *offsets;
    int i;
};

static void fill_nak_array(void *element, void *args){
    struct offset_holder *holder = (struct offset_holder *)args;
    
    Offset *offset = (Offset *)element;

    holder->offsets[holder->i].start = htonl(offset->start);
    holder->offsets[holder->i].end = htonl(offset->end);
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

uint8_t build_ack(char*packet, uint32_t start, uint8_t type, Request *req) {
    packet[start] = ACK_PDU;
    uint32_t packet_index = start + 1;
    Pdu_ack *pdu_ack = (Pdu_ack *) &packet[packet_index];
    pdu_ack->directive_code = type;
    pdu_ack->directive_subtype_code = ACK_FINISHED_END;
    pdu_ack->condition_code = COND_NO_ERROR;
    packet_index += 2;
    return packet_index - start;
}

static void process_pdu_eof(char *packet, Request *req) {

    Pdu_eof *eof_packet = (Pdu_eof *) packet;
    if (eof_packet->checksum == req->file->partial_checksum) {

        uint32_t file_size = ntohl(eof_packet->file_size);

        ssp_printf("received eof packet, filesize: %u\n", file_size);

        req->file->missing_offsets->print(req->file->missing_offsets, print, NULL);
        req->file->eof_checksum = eof_packet->checksum;
        
        //if transmission mode unacknowledged, close file 
        if (req->transmission_mode == 0) {
            return;
        }
        
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

//sets destination id in request as the incomming source id, sets transaction number 
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

    if (p_state->my_cfdp_id != dest_id){
        ssp_printf("someone is sending packets here that are not for my id %u, dest_id: %u\n", p_state->my_cfdp_id, dest_id);
        return -1;
    }

    uint16_t packet_data_len = ntohs(header->PDU_data_field_len);

    req->packet_data_len = packet_data_len;
    req->transmission_mode = header->transmission_mode;
    req->dest_cfdp_id = source_id;
    req->transaction_sequence_number = transaction_sequence_number;

    return packet_index;

}

static void write_packet_data_to_file(char *data_packet, uint32_t data_len,  File *file) {
    if(file == NULL) {
        ssp_error("file struct is null, can't write to file");
        ssp_printf("dropped metadata packet :( we have no fix for this yet\n " );
        return;

    }
    File_data_pdu_contents *packet = (File_data_pdu_contents *)data_packet;
    uint32_t offset_start = packet->offset;
    uint32_t offset_end = offset_start + data_len - 4;
    
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

    return;
}

uint8_t ack_response(char *packet, uint32_t packet_index, Request *req){

    Pdu_ack *pdu_ack = (Pdu_ack *) &packet[packet_index];
    ssp_printf("received ack!\n");
    if (pdu_ack->directive_code == EOF_PDU) {


        ssp_printf("received eof ack!\n");
    }
    else if (pdu_ack->directive_code == FINISHED_PDU) {
        req->type = finished;
        ssp_printf("received finished ack\n");

    }
}


/*------------------------------------------------------------------------------

                                    REMOTE SIDE
                                    aka: handles responses from server

------------------------------------------------------------------------------*/


static int nak_response(char *packet, uint32_t start, Request *req, Response res, Client *client) {
        uint32_t packet_index = start;
        Pdu_nak *nak = (Pdu_nak *) &packet[packet_index];
        uint32_t offset_first = ntohl(nak->start_scope);
        uint32_t offset_last = ntohl(nak->end_scope);
        uint64_t segments = ntohll(nak->segment_requests);
        packet_index += 16;

        if (req->buff == NULL){
            ssp_printf("req->buff is null\n");
        }
        if (client->pdu_header == NULL){
            ssp_printf("pdu_header is null\n");
        }

        uint32_t outgoing_packet_index = build_pdu_header(req->buff, req->transaction_sequence_number, 0, client->pdu_header);
        uint32_t offset_start = 0;
        uint32_t offset_end = 0;

        printf("number of segments to resend: %u\n", segments);
        for (int i = 0; i < segments; i++){
            //outgoing_packet_index
            memcpy(&offset_start, &packet[packet_index], 4);
            offset_start = ntohl(offset_start);
            packet_index += 4;
            memcpy(&offset_end, &packet[packet_index], 4);
            offset_end = ntohl(offset_end);
            packet_index += 4;

            printf("offset sending first %u last %u\n", offset_start, offset_end);
            printf("first %u and end %u\n", offset_first, offset_last);

            build_nak_response(req->buff, outgoing_packet_index, offset_start, req, client);
            ssp_sendto(res);
        }
        
        return packet_index;

}


//fills the current request with packet data, responses from servers
void parse_packet_client(char *packet, Response res, Request *req, Client* client, Protocol_state *p_state) {
 
    
    uint32_t packet_index = process_pdu_header(packet, req, p_state);
    if (packet_index == -1)
        return;

    Pdu_header *header = (Pdu_header *) packet;    
    uint16_t packet_data_len = ntohs(header->PDU_data_field_len);
    uint8_t directive = packet[packet_index];
    packet_index += 1; 

    switch(directive) {
        case FINISHED_PDU:
           if (req->type != finished) {
                ssp_printf("file successfully sent\n");
            }
            req->type = finished;
            break;
        case NAK_PDU:
            nak_response(packet, packet_index, req, res, client);
            req->type = eof;
            break;
        case ACK_PDU:
            ack_response(packet, packet_index, req);
            break;
        

        default:
            break;
    }
}


/*------------------------------------------------------------------------------

                                    SERVER SIDE
                                    aka: handles responses from remote

------------------------------------------------------------------------------*/

void on_server_time_out(Response res, Request *req, Protocol_state *p_state) {

    res.msg = req->buff;

    if (req->type == finished)
        return;

    if (req->buff == NULL) {
        ssp_printf("req buffer is null, couldn't process timeout request\n");
    }

    uint8_t start = build_pdu_header(res.msg, req->transaction_sequence_number, 1, req->pdu_header);
    uint16_t data_len = 0;
    Pdu_header *pdu_header = (Pdu_header *) &res.msg;
  
    if (req->file == NULL) {
        ssp_printf("file is null, couldn't process timeout request\n");
        return;
    }

    //send missing NAKS
    if (req->file->missing_offsets->count > 0) {
        build_nak_packet(res.msg, start, req);
        ssp_sendto(res);
        return;
    }

    //received EOF
    if (req->file->eof_checksum) {
        data_len = build_ack(res.msg, start, EOF_PDU, req);
        ssp_sendto(res);
    }

    //send Finished 
    if (req->file->eof_checksum == req->file->partial_checksum && req->file->missing_offsets->count == 0) {
        data_len = build_finished_pdu(res.msg, start, req);
        ssp_sendto(res);
    }


}

//fills the current_request struct for the server, incomming requests
void parse_packet_server(char *packet, uint32_t packet_len, Response res, Request *req, Protocol_state *p_state) {

    uint32_t packet_index = process_pdu_header(packet, req, p_state);
    if (packet_index == -1)
        return;

     Pdu_header *header = (Pdu_header *) packet;

    //process file data
    if (header->PDU_type == 1) {
        write_packet_data_to_file(&packet[packet_index], req->packet_data_len, req->file);
        return;
    }
    
    //set header for responding later
    if (req->pdu_header == NULL){
        req->pdu_header = get_header_from_mib(p_state->mib, req->dest_cfdp_id, p_state->my_cfdp_id);
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

        case ACK_PDU: 
            ack_response(packet, packet_index, req);

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

    if (res.msg == NULL) {
        ssp_printf("req->buff is null, couldn't process user request\n");
        return;
    }
    memset(res.msg, 0, client->packet_len);

    uint32_t start = build_pdu_header(res.msg, req->transaction_sequence_number, req->transmission_mode, client->pdu_header);
    uint32_t data_len = 0;

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
            ssp_sendto(res);
            break;

        case put:
            start = build_put_packet_metadata(res, start, req, client, p_state);
            ssp_sendto(res);
            req->type = sending_data;
            break;

        case finished: 
            data_len = build_ack(res.msg, start, FINISHED_PDU, req);
            ssp_sendto(res);
            req->type = none;
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
    Request *req = client->req;
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


