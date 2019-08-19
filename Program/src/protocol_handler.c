
#include "mib.h"
#include "port.h"
#include "protocol_handler.h"
#include "string.h"
#include "packet.h"
#include "filesystem_funcs.h"
#include "requests.h"
#include "types.h"
#include "utils.h"

//snprintf
#include <stdio.h>

/*------------------------------------------------------------------------------

                                    bundled requests

------------------------------------------------------------------------------*/

static void request_metadata(Request *req, Response res) {

    ssp_printf("sending request for new metadata packet\n");
    uint8_t start = build_pdu_header(req->buff, req->transaction_sequence_number, 1, req->pdu_header);
    build_nak_directive(req->buff, start, META_DATA_PDU);
    ssp_sendto(res);
    return;
}



/*------------------------------------------------------------------------------

                                    Processing Packets

------------------------------------------------------------------------------*/

static void process_pdu_eof(char *packet, Request *req, Response res) {

    Pdu_eof *eof_packet = (Pdu_eof *) packet;

    if (!req->local_entity->Metadata_recv_indication) {
        request_metadata(req, res);
    }
    if (req->file == NULL) {
        snprintf(req->source_file_name, 75, "%s%llu%s", ".temp_", req->transaction_sequence_number, ".jpeg");
        req->file = create_temp_file(req->source_file_name);
    }


    req->local_entity->EOF_recv_indication = 1;
    req->file->eof_checksum = eof_packet->checksum;
    req->file->total_size = ntohl(eof_packet->file_size);
    
}

//TODO This needs more work, file handling when files already exist ect
int process_file_request_metadata(Request *req) {

    char temp[75];
    req->local_entity->Metadata_recv_indication = 1;

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


//for finding the struct in the list
struct request_search_params {
    uint32_t source_id;
    uint32_t transaction_sequence_number;
};

//for finding the struct in the list
static int find_request(void *element, void *args) {
    Request *req = (Request *) element;
    struct request_search_params *params = (struct request_search_params *) args;
    if (req->dest_cfdp_id == params->source_id && req->transaction_sequence_number == params->transaction_sequence_number){
      return 1;
    }
    return 0;
}


/*creates a request struct if there is none for the incomming request based on transaction sequence number or
finds the correct request struct and replaces req with the new pointer. Returns the possition in the packet 
where the data portion is, returns 0 on fail*/
int process_pdu_header(char*packet, Response res, Request **req, List *request_list, Protocol_state *p_state) {

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
        return 0;
    }

    uint16_t len = get_data_length(packet);

    Request *request = *req;
    

    //if packet is from the same request, don't' change current request
    if (request != NULL && request->transaction_sequence_number == transaction_sequence_number && request->dest_cfdp_id == source_id){ 
        (*req)->packet_data_len = len;         
        return packet_index;
    }

    //look for active request in list
    struct request_search_params params = {
        source_id,
        transaction_sequence_number,
    };

    Request *found_req = (Request *) request_list->find(request_list, 0, find_request, &params);

    //server side, receiving requests
    if (found_req == NULL) 
    {
        found_req = init_request(p_state->packet_size);
        ssp_printf("incoming new request\n");
        //Make new request and add it
        found_req->transmission_mode = header->transmission_mode;
        found_req->transaction_sequence_number = transaction_sequence_number;
        found_req->dest_cfdp_id = source_id;
        found_req->transaction_sequence_number = transaction_sequence_number;
        found_req->pdu_header = get_header_from_mib(p_state->mib, source_id, p_state->my_cfdp_id);
        found_req->procedure = sending_put_metadata;
        found_req->res.addr = ssp_alloc(res.size_of_addr, 1);
        memcpy(found_req->res.addr, res.addr, res.size_of_addr);
        found_req->res.packet_len = p_state->packet_size;
        found_req->res.sfd = res.sfd;
        request_list->push(request_list, found_req, transaction_sequence_number);

    } 

    found_req->packet_data_len = len;
    *req = found_req;

    return packet_index;

}

static void write_packet_data_to_file(char *data_packet, uint32_t data_len, File *file) {


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
        //uint32_t offset_first = ntohl(nak->start_scope);
        //uint32_t offset_last = ntohl(nak->end_scope);
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
            build_nak_response(req->buff, outgoing_packet_index, offset_start, req, client);
            ssp_sendto(res);
        }
        
        return packet_index;

}


//fills the current request with packet data, responses from servers
void parse_packet_client(char *packet, uint32_t packet_index, Response res, Request *req, Client* client) {
 

    //Pdu_header *header = (Pdu_header *) packet;    
    //uint16_t incoming_packet_data_len = ntohs(header->PDU_data_field_len);
    uint8_t directive = packet[packet_index];
    packet_index += 1; 

    switch(directive) {
        case FINISHED_PDU:
            req->local_entity->transaction_finished_indication = 1;
            req->procedure = sending_finished;
            ssp_printf("received finished pdu\n");
            break;
        case NAK_PDU:
            req->local_entity->Metadata_recv_indication = 1;
            nak_response(packet, packet_index, req, res, client);
            ssp_printf("received Nak pdu\n");
            break;
        case ACK_PDU:
            if (packet[packet_index] == EOF_PDU) {
                ssp_printf("received Eof ack\n");
                req->local_entity->EOF_recv_indication = 1;
            }
            break;
        case NAK_DIRECTIVE:
            switch (packet[packet_index])
            {
                case META_DATA_PDU:
                    ssp_printf("resending metadata\n");
                    req->procedure = sending_put_metadata;
                    break;
            
                case EOF_PDU: 
                    ssp_printf("resending eof\n");
                    req->procedure = sending_eof;
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

    if (req->resent_finished >= RESEND_FINISHED_TIMES){
        req->procedure = none;
    }
    if (req->resent_finished == RESEND_FINISHED_TIMES) {
        ssp_printf("file successfully sent\n");
        req->resent_finished = RESEND_FINISHED_TIMES + 1;
    }
}

//current user request, to send to remote
void user_request_handler(Response res, Request *req, Client* client) {

    if (req == NULL)
        return;

    uint32_t start = build_pdu_header(req->buff, req->transaction_sequence_number, req->transmission_mode, client->pdu_header);

    check_req_status(req, client);
    //Response res = req->res;

    switch (req->procedure)
    {
        case sending_eof: 
            ssp_printf("sending eof\n");
            req->procedure = none;
            build_eof_packet(req->buff, start, req);
            req->local_entity->EOF_sent_indication = 1;
            ssp_sendto(res);
            break;

        case sending_data: 
            if (req->local_entity->EOF_sent_indication == 1)
                return;
            
            if (build_data_packet(req->buff, start, req->file, client->packet_len)) {
                req->procedure = sending_eof;
                ssp_printf("sending data blast\n");
            }
            ssp_sendto(res);
            break;

        case sending_put_metadata:
            ssp_printf("sending metadata\n");
            start = build_put_packet_metadata(res, start, req);
            ssp_sendto(res);
            req->procedure = sending_data;
            break;

        case sending_finished:
            ssp_printf("sending finished packet\n");
            build_ack(req->buff, start, FINISHED_PDU);
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

static void reset_timeout(Request *req) {

    uint8_t time = req->timeout++;
    //ssp_printf("timeout %u for id: %u sequence: %u\n", time, req->dest_cfdp_id, req->transaction_sequence_number);
    if (time == 20) {
        req->timeout = 0;
        req->procedure = clean_up;
        ssp_printf("timeout, cleaning up request\n");
    }
}

/*
static void print_offsets(void *element, void *args) {

    Offset *off = (Offset *) element;
    ssp_printf("missing offset start: %d end:%d\n", off->start, off->end);
}
*/

void on_server_time_out(Response res, Request *req) {

    if (req == NULL)
        return;

    reset_timeout(req);

    if (req->procedure == none)
        return;
    
    if (req->transmission_mode == 1)
        return; 

    uint8_t start = build_pdu_header(req->buff, req->transaction_sequence_number, 1, req->pdu_header);

    if (req->resent_finished == RESEND_FINISHED_TIMES) {
        req->procedure = none;
        ssp_printf("file sent, closing request\n");
        return;
    }

    //send request for metadata
    if (!req->local_entity->Metadata_recv_indication) {
        ssp_printf("sending request for new metadata packet\n");
        build_nak_directive(req->buff, start, META_DATA_PDU);
        ssp_sendto(res);
        return;
    }

    //send missing eofs
    if (!req->local_entity->EOF_recv_indication) {
        build_nak_directive(req->buff, start, EOF_PDU);
        ssp_sendto(res);
    }

    //send missing NAKS
    if (req->file->missing_offsets->count > 0) {
        ssp_printf("sending Nak data\n");
        build_nak_packet(req->buff, start, req);
        ssp_sendto(res);
        return;

    } else {

        if (req->file->eof_checksum == req->file->partial_checksum){
            ssp_printf("sending finsihed pdu\n");
            build_finished_pdu(req->buff, start);
            ssp_sendto(res);
            req->resent_finished++;   
            return;
        }
        else {
            ssp_printf("checksum have: %u checksum_need: %u\n", req->file->partial_checksum, req->file->eof_checksum);
        }
    }
    //received EOF, send back 3 eof ack packets
    if (req->local_entity->EOF_recv_indication && req->resent_eof < RESEND_EOF_TIMES) {
        ssp_printf("sending eof ack\n");
        build_ack(req->buff, start, EOF_PDU);
        ssp_sendto(res);
        req->resent_eof++;
    }
}

//fills the current_request struct for the server, incomming requests
void parse_packet_server(char *packet, uint32_t packet_index, Response res, Request *req, Protocol_state *p_state) {

    if (packet_index == 0)
        return;
        
    Pdu_header *header = (Pdu_header *) packet;

    req->timeout = 0;

    //process file data
    if (header->PDU_type == 1) {
        if (!req->local_entity->Metadata_recv_indication) {
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
            if (req->local_entity->Metadata_recv_indication)
                break;

            req->procedure = sending_put_metadata;
            ssp_printf("received metadata packet\n");
            fill_request_pdu_metadata(&packet[packet_index], req);
            process_file_request_metadata(req);
            break;
    
        case EOF_PDU:
            if (req->local_entity->EOF_recv_indication)
                break;;

            if (!req->local_entity->Metadata_recv_indication)
                request_metadata(req, res);
            
            ssp_printf("received eof packet\n");
            process_pdu_eof(&packet[packet_index], req, res);

            break;

        case ACK_PDU: 
            ssp_printf("received Ack\n");
            Pdu_ack* ack_packet = (Pdu_ack *) &packet[packet_index]; 
            if (ack_packet->directive_code == FINISHED_PDU) {
                ssp_printf("received finished packet\n");
                req->local_entity->transaction_finished_indication = 1;
            }
            break;
        default:
            break;
    }
}

