#include <stdint.h>


#ifndef PACKET_H
#define PACKET_H

#define PACKET_HEADER_LEN 224

// PACKET_HEADER_LEN + data max size = (65536 * 8)
#define PACKET_MAX_LEN 524512
#define PACKET_LEN 1024 + 224 // 1248 limiting the data portion to 56 bytes



typedef struct pdu_header{
    unsigned int version: 3;

    //0 File directive, 1 File Data,
    unsigned int PDU_type: 1;  

    //0 toward file receiver, 1 toward file sender
    unsigned int direction: 1;
    
    //0 acknowledged, 1 unacknowledged
    unsigned int transmission_mode: 1;

    //0 crc not present, 1 crc present
    unsigned int CRC_flag: 1; 

    //set to 0
    unsigned int reserved_bit_0: 1; 
    
    //data field length in octets
    unsigned int PDU_data_field_len: 16;

    //set to 0
    unsigned int reserved_bit_1: 1;
    
    //length of entity ids in octets
    unsigned int length_of_entity_IDs: 3;

     //set to 0
    unsigned int reserved_bit_2: 1;

    //number of octets in sequence number, 0 is one octet
    unsigned int transaction_seq_num_len: 3; 

    //variable in size, and will get memory allocated based on the length variables above
    void *source_id;

    //variable in size, and will get memory allocated based on the length variables above
    void *transaction_sequence_number;

    //variable in size, and will get memory allocated based on the length variables above
    void *destination_id;
} Pdu_header;  

typedef struct originating_tranaction_id_message {
    
    unsigned int reserved_bit_0 : 1;
    unsigned int length_of_entity_id : 3;
    unsigned int reserved_bit_1 : 1;
    unsigned int length_of_sequence_number: 3;

    //variable length of bits
    void *source_id;
    void *transaction_sequence_number;

} Originating_tranaction_id_message;


/*------------------------------------------------------------------------------
    The following are the different TLV parameters and types associated
    filestore_request:  type 0x00
                        value: struct filestore_request
    filestore_response: type 0x01
                        value: struct filestore_response

    //TODO 
    Messages to user.
    Fault Handler overrides.
    Flow Label. 

------------------------------------------------------------------------------*/


typedef struct lv {
    unsigned int length : 8;
    // size is 8 * length
    void *value;
} LV;

typedef struct tlv {
    unsigned int type : 8;
    unsigned int length : 8;
    // size is 8 * length
    void *value;
} TLV;


//---------------------------filestore_request----------------------------------
//action codes
#define CREATE_FILE 0x00
#define DELETE_FILE 0x01
#define RENAME_FILE 0x02 //second filename present
#define APPEND_FILE 0x03 //second filename present
#define REPLACE_FILE 0x04 //second filename present
#define CREATE_DIRECTORY 0x05
#define REMOVE_DIRECTORY 0x06
#define DENY_FILE 0x07 //delete if present
#define DENY_DIRECTORY 0x08 //delete if present

// The type for this TLV is 0x00
typedef struct filestore_request {
    unsigned int action_code: 4;
    unsigned int spare : 4;
    LV first_file_name;
    LV second_file_name; //only present in #RENAME_FILE, APPEND_FILE, REPLACE_FILE


} Filestore_request;


//---------------------------filestore_response---------------------------------
#define SUCCESS 0x00
#define CREATE_NOT_ALLOWED 0x01
#define FILE_DOES_NOT_EXIST 0x01
#define OLD_FILE_NAME_DOES_NOT_EXIST 0x01
#define DELETE_NOT_ALLOWED 0x02
#define NEW_FILE_NAME_ALREADY_EXISTS 0x02
#define RENAME_NOT_ALLOWED 0x04
#define FILE_NAME_1_DOES_NOT_EXIST 0x01
#define FILE_NAME_2_DOES_NOT_EXIST 0x02
#define APPEND_NOT_ALLOWED 0x04
#define REPLACE_NOT_ALLOWED 0x04
#define DIRECTORY_CANNOT_BE_CREATED 0x01
#define DIRECTORY_DOES_NOT_EXIST 0x01
#define NOT_PERFORMED 0x0F


// The type for this TLV is 0x01
typedef struct filestore_response {
    unsigned int action_code: 4;
    unsigned int status_code : 4;
    LV first_file_name;
    LV second_file_name; //only present in #RENAME_FILE, APPEND_FILE, REPLACE_FILE

    LV filestore_message;
    /*LV length field indicates zero length and LV value field is
        omitted when there is no Filestore Message parameter present. */

} Filestore_response;



/*------------------------------------------------------------------------------
    The following are the different PDUs, PDUs are what is included in the 
    data portion of the PDU_header these include:

        File directive PDUs:
            File directive PDUs all start with a  1 byte directive code,
            followed by one of the following various pdu types:

            - pdu_meta_data 
            


------------------------------------------------------------------------------*/

//directive_codes 00 - 03, and 0D-FF are reserved
#define EOF_PDU 0x04
#define FINISHED_PDU 0x05
#define ACK_PDU 0x06
#define META_DATA_PDU 0x07
#define NAK_PDU 0x08
#define PROMPT_PDU 0x09
#define KEEP_ALIVE_PDU 0x0C


typedef struct pdu_directive {
    uint8_t directive_code;
    void *pdu_type;
} Pdu_directive;


typedef struct pdu_meta_data {
    
    unsigned int segmentation_control : 1; //0 Record boundaries respeced, 1 not respected
    unsigned int reserved_bits: 7;
    unsigned int file_size : 32; //length of the file in octets, set all 0 for unbounded size
    
    LV source_file;
    LV destination_file;

    /*
    Options include:
        Filestore requests, 
        Messages to user.
        Fault Handler overrides.
        Flow Label. 
    */

    TLV *options;

} Pdu_meta_data;

#endif 