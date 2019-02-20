#include <stdint.h>


#ifndef PACKET
#define PACKET 


typedef struct PDU_header{
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
} PDU_header;  



#endif 