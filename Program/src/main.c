
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include <unistd.h>
#include "protocol_handler.h"
#include "utils.h"



int main() {

    Protocol_state *p_state = ssp_connectionless_server("1111");
    //p_state->client_list = linked_list();

    sleep(1);
    Client *client = ssp_connectionless_client("127.0.0.1", "1111", p_state);
    //p_state->client_list->add(p_state->client_list, client);
    
    ssp_cleanup_client(client);
    ssp_cleanup(p_state);
    return 0;
}

