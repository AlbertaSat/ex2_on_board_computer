#include <stdint.h>


#ifndef PACKET_H
#define PACKET_H

#define PACKET_HEADER_LEN 224

// PACKET_HEADER_LEN + data max size = (65536 * 8)
#define PACKET_MAX_LEN 524512
#define PACKET_LEN 1024 + 224 // 1248 limiting the data portion to 56 bytes

typedef struct pdu_header{
    unsigned int version: 3; 
    unsigned int PDU_type: 1;  //0 File directive, 1 File Data, 
    unsigned int direction: 1; //0 toward file receiver, 1 toward file sender
    unsigned int transmission_mode: 1; //0 acknowledged, 1 unacknowledged
    unsigned int CRC_flag: 1; //0 crc not present, 1 crc present
    unsigned int reserved_bit_0: 1; //set to 0
    unsigned int PDU_data_field_len: 16;
    unsigned int reserved_bit_1: 1; //set to 0
    unsigned int length_of_entity_IDs: 3;
    unsigned int reserved_bit_2: 1;
    unsigned int transaction_seq_num_len: 3; //number of octets in sequence number, 0 is one octet

    //these are variable in size, and will get memory allocated based on the length variables above
    void *source_id;
    void *transaction_seq_num;
    void *destination_id;
} Pdu_header;  

typedef struct packet {
    Pdu_header header;
    uint8_t *data;
    uint8_t *crc;
} Packet;

#endif 