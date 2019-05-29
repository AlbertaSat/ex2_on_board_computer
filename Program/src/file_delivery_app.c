
#include "mib.h"
#include "protocol_handler.h"
#include "port.h"
#include "stddef.h"
#include "stdint.h"
#include "string.h"
#include "file_delivery_app.h"

Protocol_state  *init_ftp(uint32_t my_cfdp_address) {

    //Memory information base
    MIB *mib = init_mib();

    //setting host name for testing
    char *host_name = "127.0.0.1";
    uint32_t addr[sizeof(uint32_t)];
    inet_pton(AF_INET, host_name, addr);
    
    //adding new cfdp entities to management information base
    add_new_cfdp_entity(mib, 1, *addr, 1111);
    add_new_cfdp_entity(mib, 2, *addr, 1112);   
    add_new_cfdp_entity(mib, 3, *addr, 1113);   
    add_new_cfdp_entity(mib, 4, *addr, 1114);   

    //find server client in mib
    Remote_entity* server_entity = mib->remote_entities->find(mib->remote_entities, my_cfdp_address, NULL, NULL);
    if (server_entity == NULL) {
        printf("couldn't find your id in the information base\n");
    }
    
    char port[20];
    snprintf(port, 20, "%u", server_entity->UT_port);

    Protocol_state *p_state = ssp_alloc(sizeof(Protocol_state), 1);
    p_state->packet_size = PACKET_LEN;
    p_state->my_cfdp_id = my_cfdp_address;
    p_state->mib = mib;


    p_state->server_port = ssp_alloc(sizeof(char), 10);
    checkAlloc(p_state->server_port, 1);

    strncpy ((char*)p_state->server_port, port, 10);

    p_state->request_list = linked_list();
    p_state->current_server_request = init_request(p_state->packet_size);

    return p_state;
}