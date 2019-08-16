#ifndef SSP_TYPES_H
#define SSP_TYPES_H

#include "stdint.h"
#include "packet.h"
#include "list.h"

#include <stddef.h>


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
    Local_entity server;
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


typedef enum Indication {
    finished, 
    none,
    transaction,
    put,
    eof,
    //sending data
    sending_data,
    clean_up

} Indication;

//incoming requests spin up requests
typedef struct request {
    Indication type;
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

    uint8_t received_metadata;
    uint8_t received_eof;
    //counter for resending eof packets == 3
    uint8_t resent_eof;
    uint8_t received_finished;
    //counter for resending finished packets == 3
    uint8_t resent_finished;
    //bool for sending first blast of data packets
    uint8_t sent_first_data_round;

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

    //can get rid of these, these are found in mib_info anyway
    uint32_t cfdp_id;
    uint32_t unitdata_id;
    uint32_t unitdata_port;
    
    Remote_entity *mib_info;
    Pdu_header *pdu_header;
    
    Protocol_state *p_state;    

    //bool for exiting the client thread
    uint8_t close;


} Client;





#endif

