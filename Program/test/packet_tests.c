
#define TESTING 1

#include "test.h"
#include "utils.h"
#include <stdio.h>
#include "filesystem_funcs.h"
#include "port.h"

#define PACKET_TEST_SIZE 2000 


static int test_respond_to_naks(char *packet, uint32_t packet_index) {
    Request *req = init_request(5000);

    ssp_cleanup_req(req);

}

static int test_respond_metadata_request() {
    
    //ssp_printf("input a src file:\n");
    Client *client = ssp_connectionless_client(conf->client_cfdp_id, p_state);
    
    //send via acknoleged mode //0 acknowledged, 1 unacknowledged
    //put_request("test.txt", "delivered_file.txt", 0, 0, 0, 0, NULL, NULL, new_client, p_state);
    put_request("pic.jpeg", "remote_pic.jpeg", 0, 0, 0, 0, NULL, NULL, client, p_state);

    uint32_t start = build_pdu_header(res.msg, req->transaction_sequence_number, req->transmission_mode, client->pdu_header);
    uint32_t data_len = build_put_packet_metadata(res, packet_index, req, client, p_state);
    set_data_length(res.msg, data_len);
}


static void nak_print(void *element, void *args){
    Offset *offset = (Offset *)element;
    ssp_printf("start: %u end: %u\n", offset->start, offset->end);
}

static int test_build_nak_packet(char* packet, uint32_t start) {
    printf("testing nak creation 1 \n");
    Request *req = init_request(5000);


    req->file_size = 100000;
    memcpy(req->destination_file_name, "testestest", 15);
    memcpy(req->source_file_name, "someotherfile", 15);

    process_file_request_metadata(req);

    uint64_t count = req->file->missing_offsets->count;
    ssp_printf("missing offset number=%u\n", count);

    uint32_t data_len = build_nak_packet(packet, start, req);

    ASSERT_EQUALS_INT("NAK directive code set", packet[start], NAK_PDU);
    //25 = start_scope + end_scope + 1 offset + 1byte NAK_PDU code
    ASSERT_EQUALS_INT("length of packet", 25, data_len);
    uint32_t packet_index = start + 1;
    Pdu_nak *nak = (Pdu_nak *) &packet[packet_index];
    
    uint32_t start_scope = ntohl(nak->start_scope);
    uint32_t end_scope = ntohl(nak->end_scope);

    ASSERT_EQUALS_INT("start offset == 0 ", start_scope, 0);
    ASSERT_EQUALS_INT("end scope == 100000 ", end_scope, 100000);

    uint64_t number_of_segments = ntohll(nak->segment_requests);
    ASSERT_EQUALS_INT("number of segments == 1 ", number_of_segments, 1);

    Offset offset[count];
    memcpy(offset, &nak->segments, sizeof(Offset) * count);
    start_scope = ntohl(offset->start);
    end_scope = ntohl(offset->end);

    ASSERT_EQUALS_INT("start offset == 0 ", start_scope, 0);
    ASSERT_EQUALS_INT("end scope == 100000 ", end_scope, 100000);

    //add a bunch more  
    printf("testing nak creation 2 \n");

    receive_offset(req->file, 0, 1250, 5000);
    receive_offset(req->file, 0, 6000, 9000);
    receive_offset(req->file, 0, 10000, 15000);

    data_len = build_nak_packet(packet, start, req);
    set_data_length(packet, data_len);

    number_of_segments = ntohll(nak->segment_requests);

    ASSERT_EQUALS_INT("number of segments == 4 ", number_of_segments, 4);

    start_scope = ntohl(nak->start_scope);
    end_scope = ntohl(nak->end_scope);
    ASSERT_EQUALS_INT("correct packet start", start_scope, 0);
    ASSERT_EQUALS_INT("correct packet end", end_scope, 100000);
    packet_index += 16;

    //outgoing_packet_index
    memcpy(&start_scope, &packet[packet_index], 4);
    start_scope = ntohl(start_scope);
    packet_index += 4;
    memcpy(&end_scope, &packet[packet_index], 4);
    end_scope = ntohl(end_scope);
    packet_index += 4;
    ASSERT_EQUALS_INT("correct packet offset 0 start", start_scope, 0);
    ASSERT_EQUALS_INT("correct packet offset 0 end", end_scope, 1250);
    
    memcpy(&start_scope, &packet[packet_index], 4);
    start_scope = ntohl(start_scope);
    packet_index += 4;
    memcpy(&end_scope, &packet[packet_index], 4);
    end_scope = ntohl(end_scope);
    packet_index += 4;
    ASSERT_EQUALS_INT("correct packet offset 1 start", start_scope, 5000);
    ASSERT_EQUALS_INT("correct packet offset 1 end", end_scope, 6000);

    memcpy(&start_scope, &packet[packet_index], 4);
    start_scope = ntohl(start_scope);
    packet_index += 4;
    memcpy(&end_scope, &packet[packet_index], 4);
    end_scope = ntohl(end_scope);
    packet_index += 4;
    ASSERT_EQUALS_INT("correct packet offset 2 start", start_scope, 9000);
    ASSERT_EQUALS_INT("correct packet offset 2 end", end_scope, 10000);

    memcpy(&start_scope, &packet[packet_index], 4);
    start_scope = ntohl(start_scope);
    packet_index += 4;
    memcpy(&end_scope, &packet[packet_index], 4);
    end_scope = ntohl(end_scope);
    packet_index += 4;
    ASSERT_EQUALS_INT("correct packet offset 3 start", start_scope, 15000);
    ASSERT_EQUALS_INT("correct packet offset 3 end", end_scope, 100000);

    Pdu_header *header = (Pdu_header*) packet;
    ssp_printf("datalen %u\n", header->PDU_data_field_len);
    ssp_printf("datalen %u\n", data_len);

    ASSERT_EQUALS_INT("correct packet data_len", data_len, header->PDU_data_field_len);

    ssp_cleanup_req(req);
    return 1;
}

int test_build_ack_finished_pdu(char *packet, uint32_t start) {
    printf("testing finished ack creation\n");
    Request *req;

    Pdu_directive *pdu_d = &packet[start];
    ASSERT_EQUALS_INT("ACK_PDU directive correct", pdu_d->directive_code, ACK_PDU);

    Pdu_ack *ack = (Pdu_ack *)&packet[start + 1];
    ASSERT_EQUALS_INT("EOF_PDU directive correct", EOF_PDU, ack->directive_code);
    ASSERT_EQUALS_INT("COND_NO_ERROR correct", COND_NO_ERROR, ack->condition_code);
    ASSERT_EQUALS_INT("ACK_FINISHED_END correct", ack->directive_subtype_code, ACK_FINISHED_END);
}


int test_build_ack_eof_pdu(char *packet, uint32_t start) {
    //empty request
    printf("testing eof ack creation\n");

    Request *req;
    uint8_t len =  build_ack (packet, start, EOF_PDU, req);

    Pdu_directive *pdu_d = &packet[start];
    ASSERT_EQUALS_INT("ACK_PDU directive correct", pdu_d->directive_code, ACK_PDU);

    Pdu_ack *ack = (Pdu_ack *)&packet[start + 1];
    ASSERT_EQUALS_INT("EOF_PDU directive correct", EOF_PDU, ack->directive_code);
    ASSERT_EQUALS_INT("COND_NO_ERROR correct", COND_NO_ERROR, ack->condition_code);
    ASSERT_EQUALS_INT("ACK_FINISHED_END correct", ack->directive_subtype_code, ACK_FINISHED_END);
}

int test_build_pdu_header_header(char *packet, Pdu_header *header, uint64_t sequence_number) {


    printf("testing header creation\n");
    uint8_t length = build_pdu_header(packet, sequence_number, 0, header);
    uint32_t packet_index = PACKET_STATIC_HEADER_LEN;

    ASSERT_EQUALS_INT("packet length: ", length, 9);
    ASSERT_EQUALS_STR("packet source id ", &packet[packet_index], header->source_id, header->length_of_entity_IDs);
    packet_index += header->length_of_entity_IDs;

    ASSERT_NOT_EQUALS_INT("transaction_sequence_number correctly placed ", &packet[packet_index], sequence_number);
    packet_index += header->transaction_seq_num_len;

    ASSERT_NOT_EQUALS_STR("packet destination not equal to source id ", &packet[packet_index], header->source_id, header->length_of_entity_IDs);
    ASSERT_EQUALS_STR("packet destination id ", &packet[packet_index], header->destination_id, header->length_of_entity_IDs);

    packet_index += header->length_of_entity_IDs;
    Pdu_header *new_header = packet;

    ASSERT_EQUALS_INT("CRC = CRC", header->CRC_flag, new_header->CRC_flag);
    ASSERT_EQUALS_INT("direction = direction", header->direction, new_header->direction);
    ASSERT_EQUALS_INT("length_of_entity_IDs = length_of_entity_IDs ", header->length_of_entity_IDs, new_header->length_of_entity_IDs);
    ASSERT_EQUALS_INT("PDU data_field_len = PDU_data_field_len ", header->PDU_data_field_len, new_header->PDU_data_field_len);
    ASSERT_EQUALS_INT("PDU_type = PDU_type", header->PDU_type, new_header->PDU_type);
    ASSERT_EQUALS_INT("reserved_bit_0 = reserved_bit_0 ", header->reserved_bit_0, new_header->reserved_bit_0);
    ASSERT_EQUALS_INT("reserved_bit_1 = reserved_bit_1 ", header->reserved_bit_1, new_header->reserved_bit_1);
    ASSERT_EQUALS_INT("reserved_bit_2 = reserved_bit_2 ", header->reserved_bit_2, new_header->reserved_bit_2);
    ASSERT_EQUALS_INT("version = version", header->version, new_header->version);
    ASSERT_EQUALS_INT("transaction_seq_num_len = transaction_seq_num_len ", header->transaction_seq_num_len,  new_header->transaction_seq_num_len);
    ASSERT_EQUALS_INT("transmission_mode = transmission_mode ", 0, new_header->transmission_mode);
    ASSERT_EQUALS_INT("total header length equal ", packet_index, length);

    return packet_index;
}

int packet_tests(Pdu_header *header) {

    printf("starting Packet Tests (creating and changing packet values)\n");
    char *packet = calloc(PACKET_TEST_SIZE, sizeof(char));
    uint64_t sequence_number = 12345663234;
    int packet_index = test_build_pdu_header_header(packet, header, sequence_number);
    test_build_ack_eof_pdu(packet, packet_index);
    test_build_nak_packet(packet, packet_index);
    test_respond_to_naks(packet, packet_index);


    ssp_free(packet);

}