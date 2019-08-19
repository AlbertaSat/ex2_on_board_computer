
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//Have to include these files

//for types
#include "protocol_handler.h"
//for conf
#include "utils.h"
//for put request
#include "requests.h"

#include "types.h"
//for main app
#include "file_delivery_app.h"
//for signal handler, because its nice
#include "server.h"
//for ssp_thread_join, can use p_thread join on linux
#include "port.h"


//exit handler variable for the main thread
static int *exit_now;

int main(int argc, char** argv) {

    //exit handler for the main thread;
    exit_now = prepareSignalHandler();

    //get-opt configuration
    Config *conf = configuration(argc, argv);

    if (conf->my_cfdp_id == 0){
        printf("can't start server, please select an ID (-i #) and client ID (-c #) \n");
        return 1;
    }
    Protocol_state *p_state = init_ftp(conf->my_cfdp_id);
    
    ssp_connectionless_server(p_state);
        
    //create a client
    if (conf->client_cfdp_id != 0){

        //ssp_printf("input a src file:\n");
        Client *new_client = ssp_connectionless_client(conf->client_cfdp_id, p_state);
    
        //send via acknoleged mode //0 acknowledged, 1 unacknowledged
        if (conf->my_cfdp_id == 2) {
            put_request("pic.jpeg", "remote_pic1.jpeg", 0, 0, 0, 1, NULL, NULL, new_client, p_state);
            put_request("pic.jpeg", "remote_pic2.jpeg", 0, 0, 0, 0, NULL, NULL, new_client, p_state);
        }
        if (conf->my_cfdp_id == 3)
            put_request("pic.jpeg", "remote_pic3.jpeg", 0, 0, 0, 1, NULL, NULL, new_client, p_state);

        ssp_thread_join(new_client->client_handle);
        printf("client disconnected\n");
    }

    ssp_thread_join(p_state->server_handle);
    free(conf); 

    
    return 0;
}

