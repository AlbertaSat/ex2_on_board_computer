
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "types.h"

//for use
void parse_packet_server(char* buff, uint32_t packet_index, Response res, Request *req, Protocol_state *p_state);
void user_request_handler(Response res, Request *req, Client *client);
void parse_packet_client(char* buff, uint32_t packet_index, Response res, Request *req, Client *client);
void on_server_time_out(Response res, Request *current_request);
int process_pdu_header(char*packet, Response res, Request **req, List *request_list, Protocol_state *p_state);
uint8_t build_data_packet(char *packet, uint32_t start, File *file, uint32_t length);

//for testing
uint8_t build_pdu_header(char *packet, uint64_t transaction_sequence_number, uint32_t transmission_mode, Pdu_header *pdu_header);
uint8_t build_ack (char *packet, uint32_t start, uint8_t type);
int process_file_request_metadata(Request *req);
int nak_response(char *packet, uint32_t start, Request *req, Response res, Client *client);
void set_data_length(char*packet, uint16_t data_len);
uint32_t build_nak_packet(char *packet, uint32_t start, Request *req);
uint16_t get_data_length(char*packet);



#endif