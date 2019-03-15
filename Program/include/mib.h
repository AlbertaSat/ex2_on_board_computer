//the management information base (MIB)

//default faul handle
#include <stdint.h>
#include <utils.h>



#ifndef MIB_H
#define MIB_H


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


MIB *init_mib(void);
void free_mib(MIB *mib);
int add_new_cfdp_entity(MIB *mib, uint32_t cfdp_id, uint32_t UT_address, uint16_t port);



#endif