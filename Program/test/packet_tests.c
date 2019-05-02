

#include "protocol_handler.h"
#include "test.h"
#include "utils.h"
#include <stdio.h>

int test_build_ack_finished_pdu(char *packet, uint32_t start) {
    printf("testing finished ack creation\n");
    Request *req;
    uint8_t len =  test_build_ack_finished_pdu (packet, start, req);

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
    uint8_t len =  build_ack_eof_pdu (packet, start, req);

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
    char *packet = calloc(2000, sizeof(char));
    uint64_t sequence_number = 12345663234;
    int packet_index = test_build_pdu_header_header(packet, header, sequence_number);
    test_build_ack_eof_pdu(packet, packet_index);

}