
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include <unistd.h>
#include "protocol_handler.h"
#include "utils.h"
#include "list_callbacks.h"
#include "server.h"
#include "mib.h"

static int *exit_now;

int main() {

    exit_now = prepareSignalHandler();

    Protocol_state *p_state = ssp_connectionless_server("1111");
    MIB *mib = init_mib();
    p_state->mib = mib;


    //setting host name for testing
    char *host_name = "127.0.0.1";
    uint32_t addr[sizeof(uint32_t)];
    inet_pton(AF_INET, host_name, addr);
    uint16_t port = 1111;



    //adding new cfdp entity to management information base
    add_new_cfdp_entity(mib, 1, *addr, port);    
    Client *new_client = ssp_connectionless_client(1, p_state);
    Request *req = new_client->outGoing_req;


    //build a request
    req->transaction_id = 1;
    //enumerations
    req->type = put;
    req->dest_cfdp_id = "1";
    req->source_file_name = "test.txt";
    req->destination_file_name = "test.txt";
    req->segmentation_control = 0;
    req->fault_handler_overides = 0;
    req->flow_lable = 0;
    req->transmission_mode = 0;
    req->messages_to_user = "sup";
    req->filestore_requests = NULL;
    req->buff = calloc(new_client->packet_len, sizeof(char));
    
    //will block on pthread_join
    ssp_cleanup_client(new_client);
    ssp_cleanup(p_state);
    return 0;
}

