
#include "mib.h"
#include "utils.h"
#include "port.h"
#include <stddef.h>
#include "string.h"
#include "list.h"

MIB *init_mib() {
    MIB *mib = ssp_alloc(1, sizeof(MIB));
    checkAlloc(mib, 1);
    mib->remote_entities = linked_list();
    return mib;
}


static void free_mib_callback(void *element) {
    ssp_free(element);
}

void free_mib(MIB *mib){
    
    mib->remote_entities->free(mib->remote_entities, free_mib_callback);
    ssp_free(mib);
}

//these configure peers for their specific transmission configuration
int add_new_cfdp_entity(MIB *mib, uint32_t cfdp_id, uint32_t UT_address, uint16_t port){

    Remote_entity *remote = ssp_alloc(1, sizeof(Remote_entity));

    remote->CRC_required = 0;
    //these will be custom set by a config file

    remote->default_transmission_mode = 1;

    remote ->max_file_segment_len = 1200;
    checkAlloc(remote, 1);

    remote->UT_address = UT_address;
    remote->cfdp_id = cfdp_id;
    remote->UT_port = port;

    return mib->remote_entities->insert(mib->remote_entities, remote, cfdp_id);
}


Pdu_header *get_header_from_mib(MIB *mib, uint32_t dest_id, uint32_t source_id){

    Remote_entity *remote = mib->remote_entities->find(mib->remote_entities, dest_id, NULL, NULL);
    if (remote == NULL) {
        return NULL;
    }

    Pdu_header *pdu_header = ssp_alloc(1, sizeof(Pdu_header));
    checkAlloc(pdu_header, 1);

    pdu_header->reserved_bit_0 = 0;
    pdu_header->reserved_bit_1 = 0;
    pdu_header->reserved_bit_2 = 0;
    pdu_header->CRC_flag = remote->CRC_required;
    pdu_header->direction = 1;
    pdu_header->PDU_type = 0;
    pdu_header->transaction_seq_num_len = 3;
    pdu_header->length_of_entity_IDs = 1; 
    pdu_header->transmission_mode = remote->default_transmission_mode;

    pdu_header->destination_id = ssp_alloc(pdu_header->length_of_entity_IDs, sizeof(u_int8_t));
    checkAlloc(pdu_header->destination_id, 1);
    memcpy(pdu_header->destination_id, &remote->cfdp_id, pdu_header->length_of_entity_IDs);

    pdu_header->source_id = ssp_alloc(pdu_header->length_of_entity_IDs, sizeof(u_int8_t));
    checkAlloc(pdu_header->source_id, 1);
    memcpy(pdu_header->source_id, &source_id, pdu_header->length_of_entity_IDs);
    return pdu_header;
}

void ssp_cleanup_pdu_header(Pdu_header *pdu_header) {
    ssp_free(pdu_header->destination_id);
    ssp_free(pdu_header->source_id);
    ssp_free(pdu_header);
}

/*
    //building the pdu header here
    client->pdu_header = ssp_alloc(1, sizeof(Pdu_header));
    checkAlloc(client->pdu_header, 1);


    //these will need to be set with a config file, or dynamically based on the 
    //packet being sent
    
    Pdu_header* header = client->pdu_header;
    header->reserved_bit_0 = 0;
    header->reserved_bit_1 = 0;
    header->reserved_bit_2 = 0;
    header->CRC_flag = client->mib_info->CRC_required;
    header->direction = 1;
    header->PDU_type = 0;
    header->transaction_seq_num_len = 3;
    header->length_of_entity_IDs = 1; 
    header->transmission_mode = remote->default_transmission_mode;

    header->destination_id = ssp_alloc(header->length_of_entity_IDs, sizeof(u_int8_t));
    checkAlloc(header->destination_id, 1);
    memcpy(header->destination_id, &remote->cfdp_id, header->length_of_entity_IDs);

    header->source_id = ssp_alloc(header->length_of_entity_IDs, sizeof(u_int8_t));
    checkAlloc(header->source_id, 1);
    memcpy(header->source_id, &p_state->my_cfdp_id, header->length_of_entity_IDs);
    */