
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

static int *exit_now;

int main() {

    exit_now = prepareSignalHandler();

    Protocol_state *p_state = ssp_connectionless_server("1111");
    p_state->client_list = linked_list();
    Client *new_client;

/*
    for (;;) {
        if (*exit_now) {
            break;
        }
    }
*/

    

    new_client = ssp_connectionless_client("127.0.0.1", "1111", p_state);
        
     
    Request *req = calloc(1, sizeof(Request));

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

    List *requests = new_client->requests;
    requests->insert(requests, req, 1);

    //will block on pthread_join
    p_state->client_list->free(p_state->client_list, client_list_free);
    ssp_cleanup(p_state);
    return 0;
}

