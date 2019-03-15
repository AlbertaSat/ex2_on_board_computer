
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

#include "packet.h"

//for itoa
#include <stdio.h>
#include <string.h>


//exit handler variable for the main thread
static int *exit_now;


static void p(void *element, void *args) {


    Remote_entity *remote_entity = element;
    ssp_printf("[\n id: %d\n remote_ut_address: %d:%d\n]\n",
    remote_entity->cfdp_id,
    remote_entity->UT_address,
    remote_entity->UT_port
    );

}

int main(int argc, char** argv) {

    //exit handler for the main thread;
    exit_now = prepareSignalHandler();


    //get-opt configuration
    Config *conf = configuration(argc, argv);


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


    //find server client
    Remote_entity* server_entity = mib->remote_entities->find(mib->remote_entities, conf->my_cfdp_id, NULL, NULL);
    if (server_entity == NULL) {
        printf("couldn't find entity\n");
    }
    
   
    //connectionless server
    char port[17];
    
    snprintf(port, 17, "%u", server_entity->UT_port);
    printf("%s\n", port);

    Protocol_state *p_state = ssp_connectionless_server(port);
    //set this node's Id, this has to be hardcoded/assigned
    p_state->my_cfdp_id = conf->my_cfdp_id;
    p_state->mib = mib;


    //print entity data
    mib->remote_entities->print(mib->remote_entities, p, NULL);


    //create a client
    Client *new_client = ssp_connectionless_client(conf->client_cfdp_id, p_state);


    //give the client a new request to perform
    Request *req = new_client->outGoing_req;

    //build a request
    req->transaction_id = 1;
    //enumerations
    req->type = put;
    req->dest_cfdp_id = conf->client_cfdp_id;
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
    free(conf); 

    
    return 0;
}

