
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include <unistd.h>
#include "protocol_handler.h"
#include "utils.h"
#include "server.h"
#include "mib.h"
#include "requests.h"
#include "packet.h"
#include "tasks.h"
#include "file_delivery_app.h"
//for itoa
#include <stdio.h>
#include <string.h>


//exit handler variable for the main thread
static int *exit_now;

int main(int argc, char** argv) {

    //exit handler for the main thread;
    exit_now = prepareSignalHandler();

    //get-opt configuration
    Config *conf = configuration(argc, argv);

    if (conf->my_cfdp_id == 0){
        ssp_printf("can't start server, please select an ID (-i #) and client ID (-c #) \n");
        return 1;
    }
    Protocol_state *p_state = init_ftp(conf->my_cfdp_id);
    ssp_connectionless_server(p_state);

    //create a client
    if (conf->client_cfdp_id != 0){

        //ssp_printf("input a src file:\n");
        Client *new_client = ssp_connectionless_client(conf->client_cfdp_id, p_state);
    
        //send via acknoleged mode //0 acknowledged, 1 unacknowledged
        if (conf->my_cfdp_id == 2)
            put_request("pic.jpeg", "remote_pic1.jpeg", 0, 0, 0, 1, NULL, NULL, new_client, p_state);

        if (conf->my_cfdp_id == 3)
            put_request("pic.jpeg", "remote_pic2.jpeg", 0, 0, 0, 1, NULL, NULL, new_client, p_state);

        ssp_thread_join(new_client->client_handle);
        ssp_printf("client disconnected\n");
    }

    ssp_thread_join(p_state->server_handle);

    ssp_free(conf); 

    
    return 0;
}

