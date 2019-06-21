
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "filesystem_funcs.h"
#include "port.h"
#include "test.h"
#include "mib.h"
#include "packet_tests.h"
#include "filesystem_tests.h"
#include "file_delivery_app.h"
#include "protocol_handler_tests.h"

Protocol_state *init_test() {

Protocol_state *p_state = init_ftp(1);
ssp_connectionless_server(p_state);
ssp_cleanup_p_state(p_state);

}

int main () {
    
    //init_test();

    //Pdu_header *header = get_header_from_mib(mib, 2222, 1315);
    //packet_tests(header);
    //file_system_tests();
    //request_test_list_storage();
    //free_mib(mib);
    //ssp_cleanup_pdu_header(header);
    int error = 0;
    error = protocol_handler_test();


   return 0;
}