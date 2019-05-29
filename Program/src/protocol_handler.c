
#include "mib.h"
#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"
#include "filesystem_funcs.h"
#include "requests.h"


//snprintf
#include <stdio.h>

/*------------------------------------------------------------------------------

                                    creating packets

------------------------------------------------------------------------------*/
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

    packet[packet_index] = FINISHED_PDU;
    packet_index++;
    data_len++;

    //one byte
    Pdu_finished *pdu_finished = (Pdu_finished *) &packet[packet_index];
    pdu_finished->condition_code = COND_NO_ERROR;
    pdu_finished->delivery_code = 0;
    pdu_finished->file_status = FILE_RETAINED_SUCCESSFULLY;
    data_len += 1;
    packet_index += 1;

    set_data_length(packet, data_len);
    return data_len;
}


static uint8_t build_put_packet_metadata(Response res, uint32_t start, Request *req) {    
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

    uint8_t data_len = packet_index - start; 
    set_data_length(res.msg, data_len);

    return packet_index;
}
static uint8_t build_nak_response(char *packet, uint32_t start, uint32_t offset, Request *req, Client* client) {

    if (offset > req->file->total_size) {
        return 1;
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
        return 1;
    }
    
    uint16_t data_len = bytes + 4;
    set_data_length(packet, data_len);

    if (bytes <  data_size)
        return 1;

    return 0;
}


//requires a req->file to be created
//returns 1 on end of file
static uint8_t build_data_packet(char *packet, uint32_t start, File *file, uint32_t length) {

    if (file->next_offset_to_send > file->total_size){
        ssp_error("cant send an offset past the file's length\n");
        return 1;
    }

    Pdu_header *header = (Pdu_header *) packet;
    //set header to file directive 0 is directive, 1 is data
    header->PDU_type = 1;

    uint16_t packet_index = start;
    File_data_pdu_contents *packet_offset = (File_data_pdu_contents *) &packet[packet_index];
    
    //4 bytes is the size of the offset paramater
    packet_offset->offset = file->next_offset_to_send;
    packet_index += 4;

    uint16_t data_size = length - packet_index;
    
    //fill the rest of the packet with data
    int bytes = get_offset(file, &packet[packet_index], data_size, file->next_offset_to_send);
    if (bytes <= 0){
        ssp_error("could not get offset, this could because the file is empty!\n");
        return 1;
    }

    //calculate checksum for data packet, this is used to calculate in transit checksums
    file->partial_checksum += calc_check_sum(&packet[packet_index], bytes);
    file->next_offset_to_send += bytes;


    //add bytes read, and the packet offset to the data_field length
    uint16_t data_len = bytes + 4;
    set_data_length(packet, data_len);

    if (bytes <  data_size)
        return 1;

    return 0;
}

static void build_eof_packet(char *packet, uint32_t start, Request *req) {

    Pdu_header *header = (Pdu_header *) packet;
    //set header to file directive 0 is directive, 1 is data
    header->PDU_type = 0;
    
    uint8_t packet_index = (uint8_t) start;
    Pdu_directive *directive = (Pdu_directive *) &packet[packet_index];
    directive->directive_code = EOF_PDU;
    packet_index++;

    Pdu_eof *eof_packet = (Pdu_eof *) &packet[packet_index];

    //this will be need to set from the req struct probably.
    //4 bits, 
    eof_packet->condition_code = COND_NO_ERROR;
    //4 bits reserved bits
    eof_packet->spare = 0;
    packet_index++;

    //4 bytes
    eof_packet->file_size = ntohl(req->file->total_size);
    packet_index += 4;
    eof_packet->checksum = req->file->partial_checksum;
    packet_index += 4;

    //TODO addTLV fault_location
    uint16_t data_len = htons(packet_index - start);
    set_data_length(packet, data_len);

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

uint32_t build_nak_packet(char *packet, uint32_t start, Request *req) {
    
    packet[start] = NAK_PDU;
    uint32_t packet_index = start + 1;
    Pdu_nak *pdu_nak = (Pdu_nak *) &packet[packet_index];
    uint64_t count = req->file->missing_offsets->count;
    
    //fill offset array with all missing offsets [holder.offsets]
    struct offset_holder holder;
    holder.offsets = ssp_alloc(count, sizeof(Offset));
    holder.i = 0;

    req->file->missing_offsets->print(req->file->missing_offsets, fill_nak_array, &holder);
    
    pdu_nak->start_scope = holder.offsets[0].start;
    pdu_nak->end_scope = holder.offsets[holder.i-1].end;


    pdu_nak->segment_requests = htonll(count);
    packet_index+=16;
   
    memcpy(&packet[packet_index], holder.offsets, sizeof(Offset) * count);
    ssp_free(holder.offsets);

    packet_index += sizeof(Offset) * count;

    uint16_t data_len = packet_index - start;
    set_data_length(packet, data_len);

    return data_len;
}

uint8_t build_ack(char*packet, uint32_t start, uint8_t type, Request *req) {
    packet[start] = ACK_PDU;
    uint32_t packet_index = start + 1;
    Pdu_ack *pdu_ack = (Pdu_ack *) &packet[packet_index];

    pdu_ack->directive_code = type;
    pdu_ack->directive_subtype_code = ACK_FINISHED_END;
    pdu_ack->condition_code = COND_NO_ERROR;
    packet_index += 2;
    uint16_t data_len = packet_index - start;
    set_data_length(packet, data_len);

    return data_len;
}

static uint8_t build_nak_directive(char *packet, uint32_t start, uint8_t directive) {
    uint8_t data_len = 2;
    packet[start] = NAK_DIRECTIVE;
    packet[start + 1] = directive;
    
    set_data_length(packet, data_len);
    return data_len;
}

void set_data_length(char*packet, uint16_t data_len){
    Pdu_header *header = (Pdu_header*) packet;
    header->PDU_data_field_len =  htons(data_len);;
}

uint16_t get_data_length(char*packet) {
    Pdu_header *header = (Pdu_header*) packet;
    return ntohs(header->PDU_data_field_len);
}

/*------------------------------------------------------------------------------

                                    bundled requests

------------------------------------------------------------------------------*/

static void request_metadata(Request *req, Response res) {

    res.msg = req->buff;
    ssp_printf("sending request for new metadata packet\n");
    uint8_t start = build_pdu_header(res.msg, req->transaction_sequence_number, 1, req->pdu_header);
    build_nak_directive(res.msg, start, META_DATA_PDU);
    ssp_sendto(res);
    return;
}



/*------------------------------------------------------------------------------

                                    Processing Packets

------------------------------------------------------------------------------*/

static void process_pdu_eof(char *packet, Request *req, Response res) {

    Pdu_eof *eof_packet = (Pdu_eof *) packet;

    if (!req->received_metadata) {
        request_metadata(req, res);
    }
    if (req->file == NULL) {
        snprintf(req->source_file_name, 75, "%s%llu%s", ".temp_", req->transaction_sequence_number, ".jpeg");
        req->file = create_temp_file(req->source_file_name);
    }


    req->received_eof = 1;
    req->file->eof_checksum = eof_packet->checksum;
    req->file->total_size = ntohl(eof_packet->file_size);
    
}

//TODO This needs more work, file handling when files already exist ect
int process_file_request_metadata(Request *req) {

    char temp[75];
    req->received_metadata = 1;

    if (req->file == NULL)
        req->file = create_file(req->destination_file_name, 1);

    else if (req->file->is_temp) {
        snprintf(temp, 75, "%s%llu%s", ".temp_", req->transaction_sequence_number, ".jpeg");
        change_tempfile_to_actual(temp, req->destination_file_name, req->file_size, req->file);
        return 1;
    }
    
    ssp_printf("mode acknowledged, building offset map\n");
    Offset *offset = ssp_alloc(1, sizeof(Offset));
    offset->end = req->file_size;
    offset->start = 0;
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

        ssp_printf("sequence number: %u should be %u\n", p_state->my_cfdp_id, transaction_sequence_number);
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
        return;
    }

    File_data_pdu_contents *packet = (File_data_pdu_contents *)data_packet;
    uint32_t offset_start = packet->offset;
    uint32_t offset_end = offset_start + data_len - 4;
    
    if (!receive_offset(file, 0, offset_start, offset_end))
        return;

    int bytes = write_offset(file, &data_packet[4], data_len - 4, offset_start);
    if (bytes <= 0) {
        ssp_error("no new data was written\n");
        return;
    }

    file->partial_checksum += calc_check_sum(&data_packet[4], bytes);
    
    if (file->missing_offsets->count == 0)
        return;

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



/*------------------------------------------------------------------------------

                                    REMOTE SIDE
                                    aka: handles responses from server

------------------------------------------------------------------------------*/


int nak_response(char *packet, uint32_t start, Request *req, Response res, Client *client) {
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
void parse_packet_client(char *packet, Response res, Request *req, Client* client) {
 
    uint32_t packet_index = process_pdu_header(packet, req, client->p_state);
    if (packet_index == -1)
        return;

    //Pdu_header *header = (Pdu_header *) packet;    
    //uint16_t incoming_packet_data_len = ntohs(header->PDU_data_field_len);
    uint8_t directive = packet[packet_index];
    packet_index += 1; 

    switch(directive) {
        case FINISHED_PDU:
            req->received_finished = 1;
            req->type = finished;
            ssp_printf("received finished pdu\n");
            break;
        case NAK_PDU:
            req->received_metadata = 1;
            nak_response(packet, packet_index, req, res, client);
            ssp_printf("received Nak pdu\n");
            break;
        case ACK_PDU:
            if (packet[packet_index] == EOF_PDU) {
                ssp_printf("received Eof ack\n");
                req->received_eof = 1;
            }
            break;
        case NAK_DIRECTIVE:
            switch (packet[packet_index])
            {
                case META_DATA_PDU:
                    ssp_printf("resending metadata\n");
                    req->type = put;
                    break;
            
                case EOF_PDU: 
                    ssp_printf("resending eof\n");
                    req->type = eof;
                    break;
                
                default:
                    break;
            }

            break;
        default:
            break;
    }
}
static void check_req_status(Request *req, Client *client) {

    if (req->resent_finished >= 3){
        req->type = none;
    }
    if (req->resent_finished == 3) {
        ssp_printf("file successfully sent\n");
        req->resent_finished = 4;
        //ssp_cleanup_req(client->req);
        //client->req = NULL;
    }
}

//current user request, to send to remote
void user_request_handler(Response res, Request *req, Client* client) {

    if (req == NULL || req->type == none)
        return;

    if (res.msg == NULL) {
        ssp_printf("req->buff is null, couldn't process user request\n");
        return;
    }
    memset(res.msg, 0, client->packet_len);

    uint32_t start = build_pdu_header(res.msg, req->transaction_sequence_number, req->transmission_mode, client->pdu_header);

    check_req_status(req, client);
    switch (req->type)
    {
        case eof: 
            ssp_printf("sending eof\n");
            req->type = none;
            build_eof_packet(res.msg, start, req);
            ssp_sendto(res);
            break;

        case sending_data: 
            if (req->sent_first_data_round == 1)
                return;
            
            if (build_data_packet(res.msg, start, req->file, client->packet_len)) {
                req->type = eof;
                req->sent_first_data_round = 1;
                ssp_printf("sending data blast\n");
            }
            ssp_sendto(res);
            break;

        case put:
            ssp_printf("sending metadata\n");
            start = build_put_packet_metadata(res, start, req);
            ssp_sendto(res);
            req->type = sending_data;
            break;

        case finished:
            ssp_printf("sending finished packet\n");
            build_ack(res.msg, start, FINISHED_PDU, req);
            ssp_sendto(res);
            req->resent_finished++;
            break;

        default:
            break;
    }
}
/*------------------------------------------------------------------------------

                                    SERVER SIDE
                                    aka: handles responses from remote

------------------------------------------------------------------------------*/

static void reset_timeout(Protocol_state *p_state, Request *req) {

    uint32_t time = p_state->timeout++;
    ssp_printf("timeout %u\n", time);
    if (time == 20) {
        p_state->timeout = 0;
        reset_request(req);
        ssp_printf("timeout, resetting request\n");
    }
}

void on_server_time_out(Response res, Request *req, Protocol_state *p_state) {

    res.msg = req->buff;
    if (req->type == none)
        return;
   
    if (req->buff == NULL)
        ssp_printf("req buffer is null, couldn't process timeout request\n");

    reset_timeout(p_state, req);

    uint8_t start = build_pdu_header(res.msg, req->transaction_sequence_number, 1, req->pdu_header);
    //Pdu_header *pdu_header = (Pdu_header *) &res.msg;

    if (req->resent_finished == 3) {
        reset_request(req);
        ssp_printf("file sent, resetting request\n");
        return;
    }

    //send request for metadata
    if (!req->received_metadata) {
        ssp_printf("sending request for new metadata packet\n");
        build_nak_directive(res.msg, start, META_DATA_PDU);
        ssp_sendto(res);
        return;
    }

    //send missing eofs
    if (!req->received_eof) {
    build_nak_directive(res.msg, start, EOF_PDU);
        ssp_sendto(res);
    }

    ssp_printf("missing offsets %u\n", req->file->missing_offsets->count);
    //send missing NAKS
    if (req->file->missing_offsets->count > 0) {
        ssp_printf("sending Nak data\n");
        build_nak_packet(res.msg, start, req);
        ssp_sendto(res);
        return;

    } else {

        if (req->file->eof_checksum == req->file->partial_checksum){
            ssp_printf("sending finsihed pdu\n");
             build_finished_pdu(res.msg, start, req);
            ssp_sendto(res);
            req->resent_finished++;   
            return;
        }
        else {
            ssp_printf("checksum have: %u checksum_need: %u\n", req->file->partial_checksum, req->file->eof_checksum);
        }
    }
    
    //received EOF, send back 3 eof packets
    if (req->received_eof && req->resent_eof < 3) {
        ssp_printf("sending eof ack\n");
        build_ack(res.msg, start, EOF_PDU, req);
        ssp_sendto(res);
        req->resent_eof++;
    }


}

//fills the current_request struct for the server, incomming requests
void parse_packet_server(char *packet, uint32_t packet_len, Response res, Request *req, Protocol_state *p_state) {

    uint32_t packet_index = process_pdu_header(packet, req, p_state);
    if (packet_index == -1)
        return;

    Pdu_header *header = (Pdu_header *) packet;

    //set header for responding
    if (req->pdu_header == NULL){
        req->pdu_header = get_header_from_mib(p_state->mib, req->dest_cfdp_id, p_state->my_cfdp_id);
    }

    //will protbably have to timeout different clients? how does that work with ddos?
    p_state->timeout = 0;
    //process file data
    if (header->PDU_type == 1) {
        if (!req->received_metadata) {
            if (req->file == NULL) {
                snprintf(req->source_file_name, 75, "%s%llu%s", ".temp_", req->transaction_sequence_number, ".jpeg");
                ssp_printf("haven't received metadata yet, building temperary file %s\n", req->source_file_name);
                req->file = create_temp_file(req->source_file_name);
            }
            request_metadata(req, res);
        }
        write_packet_data_to_file(&packet[packet_index], req->packet_data_len, req->file);
        return;
    }
    

    Pdu_directive *directive = (Pdu_directive *) &packet[packet_index];
    packet_index++;
    switch (directive->directive_code)
    {
        case META_DATA_PDU:
            if (req->received_metadata)
                break;

            req->type = put;
            ssp_printf("received metadata packet\n");
            fill_request_pdu_metadata(&packet[packet_index], req);
            process_file_request_metadata(req);
            break;
    
        case EOF_PDU:
            if (req->received_eof)
                break;;

            if (!req->received_metadata)
                request_metadata(req, res);
            
            //this should probably just set variables for use later for calculating checksums
            ssp_printf("received eof packet\n");
            process_pdu_eof(&packet[packet_index], req, res);

            break;

        case ACK_PDU: 
            ssp_printf("received Ack\n");
            Pdu_ack* ack_packet = (Pdu_ack *) &packet[packet_index]; 
            if (ack_packet->directive_code == FINISHED_PDU) {
                ssp_printf("received finished packet\n");
                req->received_finished = 1;
            }
            break;
        default:
            break;
    }

    memset(packet, 0, packet_len);
}

