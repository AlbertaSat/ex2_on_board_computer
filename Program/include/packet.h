

#ifndef PACKET_H
#define PACKET_H
#include <stdint.h>
#define PACKET_HEADER_LEN 224
// PACKET_HEADER_LEN + data max size = (65536 * 8)
#define PACKET_MAX_LEN 524512
#define PACKET_LEN 1024 + 224 // 1248 limiting the data portion to 56 bytes
#define PACKET_STATIC_HEADER_LEN 4 //in bytes

#include "types.h"

uint8_t build_pdu_header(char *packet, uint64_t transaction_sequence_number, uint32_t transmission_mode, Pdu_header *pdu_header);
uint8_t build_finished_pdu(char *packet, uint32_t start, Request *req);
uint8_t build_put_packet_metadata(Response res, uint32_t start, Request *req);
uint8_t build_nak_response(char *packet, uint32_t start, uint32_t offset, Request *req, Client* client);
uint8_t build_data_packet(char *packet, uint32_t start, File *file, uint32_t length);

void build_eof_packet(char *packet, uint32_t start, Request *req);
void fill_nak_array(void *element, void *args);
uint32_t build_nak_packet(char *packet, uint32_t start, Request *req);
uint8_t build_ack(char*packet, uint32_t start, uint8_t type);
uint8_t build_nak_directive(char *packet, uint32_t start, uint8_t directive) ;
void set_data_length(char*packet, uint16_t data_len);
uint16_t get_data_length(char*packet);

#endif 