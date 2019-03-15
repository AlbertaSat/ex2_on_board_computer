
#include "mib.h"
#include "utils.h"
#include "port.h"


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


int add_new_cfdp_entity(MIB *mib, uint32_t cfdp_id, uint32_t UT_address, uint16_t port){

    Remote_entity *remote = ssp_alloc(1, sizeof(Remote_entity));

    remote->CRC_required = 0;
    //these will be custom set by a config file

    remote ->max_file_segment_len = 1200;
    checkAlloc(remote, 1);

    remote->UT_address = UT_address;
    remote->cfdp_id = cfdp_id;
    remote->UT_port = port;

    return mib->remote_entities->insert(mib->remote_entities, remote, cfdp_id);
}
