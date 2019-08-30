
#include "server_tests.h"
#include <stdio.h>
#include "server.h"
#include "port.h"
#include "tasks.h"


int onRecv(int sfd, char *packet, uint32_t packet_len,  uint32_t *buff_size, void *addr, size_t size_of_addr, void *other){
    printf("hello!\n");
    ssp_write(sfd, "hello!\n", 8);
    return 0;
}

int onTimeOut(void *other) {
    return 0;
}
int onStdIn(void *other){
    return 0;
}
int checkExit(void *other) {
    return 0;
}
void onExit(void *other) {
}



int server_tests(int client){

    int buffsize = 10000;
    char buff[buffsize];
    
    printf("Hello server!\n");

    if (client)
        connection_client(1111);
    else 
        connection_server("1111", buffsize, onRecv, onTimeOut, onStdIn, checkExit, onExit, NULL);

    return 0;
}

