
#include "mib.h"
#include "protocol_handler.h"
#include "port.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "file_delivery_app.h"
#include "tasks.h"
#include <stdio.h>
#include <arpa/inet.h>
#include "utils.h"

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
        ssp_printf("couldn't find your id in the information base\n");
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
    p_state->current_request = NULL;
    return p_state;
}



void ssp_connectionless_server(Protocol_state *p_state) {
    p_state->server_handle = ssp_thread_create(STACK_ALLOCATION, ssp_connectionless_server_task, p_state);
}

void ssp_connection_server(Protocol_state *p_state) {
    p_state->server_handle = ssp_thread_create(STACK_ALLOCATION, ssp_connection_server_task, p_state);
}


void ssp_connection_client(Protocol_state *p_state) {
    ssp_thread_create(STACK_ALLOCATION, ssp_connection_client_task, p_state);
}

Client *ssp_connectionless_client(uint32_t cfdp_id, Protocol_state *p_state) {


    Client *client = ssp_alloc(sizeof(Client), 1);
    checkAlloc(client, 1);

    client->current_request = NULL;
    client->request_list = linked_list();
    client->packet_len = PACKET_LEN;

    Remote_entity *remote = get_remote_entity(p_state->mib, cfdp_id);

    if (remote == NULL)
        ssp_printf("couldn't find entity in Remote_entity list\n");

    //TODO clean this up, we don't need multiple instances of UT_ports etc
    client->remote_entity = remote;
    client->pdu_header = get_header_from_mib(p_state->mib, cfdp_id, p_state->my_cfdp_id);
    client->p_state = p_state;

    client->client_handle = ssp_thread_create(STACK_ALLOCATION, ssp_connectionless_client_task, client);
    return client;
}
