
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

/*
static void p(void *element, void *args) {

    Remote_entity *remote_entity = element;
    ssp_printf("[\n id: %d\n remote_ut_address: %d:%d\n]\n",
    remote_entity->cfdp_id,
    remote_entity->UT_address,
    remote_entity->UT_port
    );
}
*/

int main(int argc, char** argv) {

    //exit handler for the main thread;
    exit_now = prepareSignalHandler();

    //get-opt configuration
    Config *conf = configuration(argc, argv);

    if (conf->my_cfdp_id == 0 || conf->client_cfdp_id == 0){
        ssp_printf("can't start server, please select an ID (-i #) and client ID (-c #) \n");
        return 1;
    }

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
    Remote_entity* server_entity = mib->remote_entities->find(mib->remote_entities, conf->my_cfdp_id, NULL, NULL);
    if (server_entity == NULL) {
        printf("couldn't find entity\n");
    }
    
   
    //connectionless server
    char port[17];
    snprintf(port, 17, "%u", server_entity->UT_port);

    Protocol_state *p_state = ssp_connectionless_server(port);  
    //set this node's Id, this has to be hardcoded/assigned
    p_state->my_cfdp_id = conf->my_cfdp_id;
    p_state->mib = mib;
    p_state->verbose_level = conf->verbose_level;

    

    //create a client
    Client *new_client = ssp_connectionless_client(conf->client_cfdp_id, p_state);
    
    //send via acknoleged mode //0 acknowledged, 1 unacknowledged
    //put_request("test.txt", "delivered_file.txt", 0, 0, 0, 0, NULL, NULL, new_client, p_state);
    put_request("pic.jpeg", "remote_pic.jpeg", 0, 0, 0, 0, NULL, NULL, new_client, p_state);

    //will block on pthread_join
    ssp_cleanup_client(new_client);
    ssp_cleanup(p_state);
    free(conf); 

    
    return 0;
}

