
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/*
#include "packet_tests.h"
#include "filesystem_tests.h"
#include "file_delivery_app.h"
#include "protocol_handler_tests.h"
#include "list_tests.h"
*/
#include "server_tests.h"

/*
Protocol_state *init_test() {

    Protocol_state *p_state = init_ftp(1);
    ssp_connectionless_server(p_state);
    ssp_cleanup_p_state(p_state);
}
*/

int main (int argc, char **argv) {
    
    //init_test();

    //Pdu_header *header = get_header_from_mib(mib, 2222, 1315);
    //packet_tests(header);
    //file_system_tests();
    //request_test_list_storage();
    //free_mib(mib);
    //ssp_cleanup_pdu_header(header);
    int error = 0;

    //error = request_tests();
    //error = protocol_handler_test();
    //error = packet_tests();
    //error = list_tests();
    //error = file_system_tests();


    if (strcmp(argv[1], "1") == 0)
        error = server_tests(0);
    else 
        error = server_tests(1);
   return error;
}