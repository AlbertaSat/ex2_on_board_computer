
#include "server_tests.h"
#include <stdio.h>
#include "server.h"
#include "port.h"
#include "tasks.h"

int server_tests(int client){

    printf("Hello server!\n");

    if (client)
        connection_client(1111);
    else 
        connection_server("1111");
    
    

    return 0;
}

