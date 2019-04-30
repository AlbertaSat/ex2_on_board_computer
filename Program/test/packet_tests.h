
#ifndef PACKET_TEST_H
#define PACKET_TEST_H


#include "protocol_handler.h"






int test_header(char *packet, Pdu_header *header, uint64_t sequence_number);
int packet_tests(Pdu_header *header);




#endif