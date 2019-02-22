
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include <unistd.h>
#include "protocol_handler.h"

int main() {


    Protocol_state* p_state = ssp_connectionless_server("1111");

    sleep(2);

    ssp_connectionless_client("127.0.0.1", "1111");
    ssp_cleanup(p_state);
    
    return 0;
}

