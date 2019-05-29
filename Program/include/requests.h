
#ifndef REQUESTS_H
#define REQUESTS_H

#include "stdint.h"
#include "protocol_handler.h"

typedef struct response {
    int sfd;
    //this is a pointer to buff in a request
    char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    //getting rid of this soon in favour of client based packet sizes
    uint32_t packet_len;
    
} Response;

typedef enum Indication {
    finished, 
    none,
    transaction,
    put,
    eof,
    //sending data
    sending_data

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

void ssp_cleanup_req(void *request);
Request *init_request(uint32_t buff_len);
void reset_request(Request *req);
#endif