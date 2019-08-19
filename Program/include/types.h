#ifndef SSP_TYPES_H
#define SSP_TYPES_H

#include "stdint.h"
#include "list.h"

#include <stddef.h>


/*-----------------------------------------------------------------------------

                    Packet structs for building packets


------------------------------------------------------------------------------*/

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

            //not implemented
            -END-OF-FILE PDU
            -FINISHED PDU 
            -ACK PDU
            -NAK PDU 
            -PROMPT PDU 
            -KEEP ALIVE PDU

            //implemented
            -METADATA PDU 
        
        Non directive PDUs (when the PDU type in the header is set to 1):
            -FILE DATA PDU 


            


------------------------------------------------------------------------------*/


#define DIRECTIVE 0
#define DATA 1

//directive_codes 00 - 03, and 0D-FF are reserved
#define EOF_PDU 0x04
#define FINISHED_PDU 0x05
#define ACK_PDU 0x06
#define META_DATA_PDU 0x07
#define NAK_PDU 0x08
#define PROMPT_PDU 0x09
#define KEEP_ALIVE_PDU 0x0C
#define SIZE_OF_DIRECTIVE_CODE 1

//custom, for missing metadata and eof
#define NAK_DIRECTIVE 0x0d

//condition codes:

#define COND_NO_ERROR 0x00
/*
‘0000’ No error
‘0001’ Positive ACK limit reached
‘0010’ Keep alive limit reached
‘0011’ Invalid transmission mode
‘0100’ Filestore rejection
‘0101’ File checksum failure
‘0110’ File size error
‘0111’ NAK limit reached
‘1000’ Inactivity detected
‘1001’ Invalid file structure
‘1010’ Check limit reached
‘1011’ – ‘1101’
CCSDS 727.0-B-4
Condition
(reserved)
‘1110’ Suspend.request received
‘1111’ Cancel.request received
*/


typedef struct pdu_directive {
    uint8_t directive_code;
} Pdu_directive;

/*
The segmentation control parameter
    - shall indicate whether the file being delivered is to be segmented as an array of octets
        or as an array of variable-length records;
    - shall be omitted when local and remote file names are omitted.
*/

typedef struct pdu_meta_data {
    //0 Record boundaries respeced (read as array of octets), 1 not respected (variable length)
    unsigned int segmentation_control : 1; 
    
    unsigned int reserved_bits: 7;

    //length of the file in octets, set all 0 for unbounded size
    uint32_t file_size;
    LV source_file_name;
    LV destination_file_name;

    /*
    Options include:
        Filestore requests, 
        Messages to user.
        Fault Handler overrides.
        Flow Label. 
    */

    TLV *options;

} Pdu_meta_data;


typedef struct file_data_pdu_contents {
    //in octets
    uint32_t offset;
    //variable sized
    unsigned char *data;
} File_data_pdu_contents;


typedef struct pdu_eof {
    unsigned int condition_code : 4;
    unsigned int spare : 4;
    uint32_t checksum;
    /* In octets. This value shall be the total number of file data octets
    transmitted by the sender, regardless of the condition code
    (i.e., it shall be supplied even if the condition code is other than
    ‘No error’). */
    uint32_t file_size;

    /*
    Omitted if condition code is ‘No error’. Otherwise, entity ID in the
    TLV is the ID of the entity at which transaction cancellation was
    initiated.*/
    TLV fault_location;
    
} Pdu_eof;


//custom, nak missing other directives, like metadata and eof
typedef struct pdu_nak_directive{
    uint8_t directive;

} pdu_nak_directive;

    /*
    start_scope is the begining of the nak requests
    end_scope is the end
    segments are
    */
typedef struct pdu_nak {
    uint32_t start_scope;
    uint32_t end_scope;

    //number of Nak_segments
    uint64_t segment_requests;
    void *segments;
} Pdu_nak;


#define ACK_FINISHED_WAYPOINT 0
#define ACK_FINISHED_END 1
#define ACK_OTHER 0

typedef struct pdu_ack {
    //Directive code of the acknowledged PDU.
    unsigned int directive_code : 4;

    /*
    Values depend on directive code. For ACK of Finished PDU: binary ‘0000’
    if Finished PDU is generated by waypoint, binary ‘0001’ if Finished
    PDU is generated by end system. (NOTE: this discrimination is
    meaningful if the Extended Procedures are implemented.)
    Binary ‘0000’ for ACKs of all other file directives.
    */
    
    unsigned int directive_subtype_code : 4;

    //Condition code of the acknowledged PDU.
    unsigned int condition_code : 4;
    unsigned int spare : 2;

    //Status of the transaction in the context of the entity that is issuing the acknowledgment.
    unsigned int transaction_status : 2;

} Pdu_ack;



//file_status codes
#define FILE_DISCARDED_DELIBERATELY 1
#define FILE_DISCARDED_DUE_TO_REJECTION 2
#define FILE_RETAINED_SUCCESSFULLY 3
#define FILE_STATUS_UNREPORTED 4


typedef struct pdu_finished {
    unsigned int condition_code : 4;

    // 0 generated by waypoint 1 generated by end system.
    unsigned int end_system_status : 1;

    //0 data complete, 1 data incomplete
    unsigned int delivery_code : 1;

    //see above
    unsigned int file_status : 2;
    TLV file_store_responses;
    TLV fault_location;

}Pdu_finished;




/*-----------------------------------------------------------------------------

                    Functions and structs for the app

------------------------------------------------------------------------------*/


typedef struct offset {
    uint32_t start;
    uint32_t end;
} Offset;


typedef struct file {
    int fd;
    uint8_t is_temp;
    uint32_t next_offset_to_send;
    uint32_t total_size;
    uint32_t partial_checksum;
    uint32_t eof_checksum;
    List *missing_offsets;

} File;


typedef struct local_entity {
    unsigned int EOF_sent_indication : 1;

    unsigned int  EOF_recv_indication : 1;

    //required
    unsigned int  file_segment_recv_indication : 1;

    //required when acting as receiving entity
    unsigned int  transaction_finished_indication : 1;
    
    //required when acting as receiving entity
    unsigned int  suspended_indication : 1;

    //required when acting as receiving entity
    unsigned int resumed_indication : 1;

    unsigned int Metadata_recv_indication: 1;
    //function pointer to default handler?
    void *default_fault_handler;

    //TODO routing information, extended procedures only, list maybe?

} Local_entity;


typedef struct remote_entity {

    //TODO probably add new UT layer specific stuff
    uint32_t UT_address;
    uint16_t UT_port;
    uint32_t cfdp_id;

    unsigned int one_way_light_time;
    unsigned int total_round_trip_allowance;
    unsigned int async_NAK_interval;
    unsigned int async_keep_alive_interval;
    unsigned int async_report_interval;
    unsigned int immediate_nak_mode_enabled : 1;
    unsigned int prompt_transmission_interval;

    //acknowledged or unacknowledged
    unsigned int default_transmission_mode: 1;

    //discard or retain (no idea what this is for yet)
    unsigned int disposition_of_incomplete : 1;

    unsigned int CRC_required : 1;
    //in octets
    unsigned int max_file_segment_len;
    //in octets
    unsigned int keep_alive_discrepancy_limit;

    //Number of expirations
    unsigned int positive_ack_timer_expiration_limit;

    //Number of expirations
    unsigned int nak_timer_expiration_limit;

    //time limit
    unsigned int transaction_inactivity_limit;




    /*
    Start of transmission opportunity A signal produced by the operating environment.
    End of transmission opportunity A signal produced by the operating environment.
    Start of reception opportunity A signal produced by the operating environment.
    End of reception opportunity A signal produced by the operating environment. 
    */
   

} Remote_entity;

typedef struct mib {
    List *remote_entities;

} MIB;


typedef struct response {
    int sfd;
    //this is a pointer to buff in a request
    char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    //getting rid of this soon in favour of client based packet sizes
    size_t size_of_addr;

    uint32_t packet_len;
    
} Response;


typedef enum procedure {
    sending_finished,
    none,
    sending_put_metadata,
    sending_eof,
    //sending data
    sending_data,
    clean_up

} Request_procedure;

//incoming requests spin up requests
typedef struct request {
    Request_procedure procedure;

    uint64_t transaction_sequence_number;
    uint32_t dest_cfdp_id;

    File *file;
    uint32_t file_size;

    char *source_file_name;
    char *destination_file_name;

    uint32_t packet_data_len;

    //timeout
    uint8_t timeout;
    
    uint8_t segmentation_control;
    uint8_t fault_handler_overides;
    uint8_t flow_lable;
    uint8_t transmission_mode;

    //counter for resending eof packets == 3
    uint8_t resent_eof;
    //counter for resending finished packets == 3
    uint8_t resent_finished;

    //bool for sending first blast of data packets
   // uint8_t sent_first_data_round;

    Remote_entity *remote_entity;
    Local_entity *local_entity;

    char* messages_to_user;
    char* filestore_requests;
    
    //sets the buffer length, for making packets
    uint32_t buff_len;

    //buffer for making packets
    char* buff;

    //header for building response packets
    Pdu_header *pdu_header;

    //handler for sending responses back
    Response res;
} Request;

typedef struct protocol_state {
    uint32_t packet_size;
    char *server_port;
    void *server_handle;
    MIB *mib;
    uint32_t my_cfdp_id;
    
    List* request_list; 

    Request *current_request;
   
    //lock this
    uint32_t transaction_sequence_number;
    
    uint8_t verbose_level;

    //bool for exiting the server thread
    uint8_t close;

} Protocol_state;


//outgoing requests spin up client threads
typedef struct client {
    
    void *client_handle;
    uint32_t packet_len;
    
    Request *current_request;
    List *request_list;

    Remote_entity *remote_entity;
    Pdu_header *pdu_header;
    
    Protocol_state *p_state;    

    //bool for exiting the client thread
    uint8_t close;


} Client;





#endif

