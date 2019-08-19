#include "types.h"
#include "packet.h"
#include "stdint.h"
#include "string.h"
#include "utils.h"
#include "port.h"
#include "filesystem_funcs.h"

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

uint8_t build_finished_pdu(char *packet, uint32_t start, Request *req) {

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


uint8_t build_put_packet_metadata(Response res, uint32_t start, Request *req) {    
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
uint8_t build_nak_response(char *packet, uint32_t start, uint32_t offset, Request *req, Client* client) {

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
//length is the total size of the packet
uint8_t build_data_packet(char *packet, uint32_t start, File *file, uint32_t length) {

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

void build_eof_packet(char *packet, uint32_t start, Request *req) {

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

void fill_nak_array(void *element, void *args){
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

    req->file->missing_offsets->iterate(req->file->missing_offsets, fill_nak_array, &holder);
    
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

uint8_t build_ack(char*packet, uint32_t start, uint8_t type) {
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

uint8_t build_nak_directive(char *packet, uint32_t start, uint8_t directive) {
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
