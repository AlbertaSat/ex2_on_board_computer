
#include "server_tests.h"
#include <stdio.h>
#include "server.h"
#include "port.h"
#include "tasks.h"


int onRecv(int sfd, char *packet, uint32_t packet_len,  uint32_t *buff_size, void *addr, size_t size_of_addr, void *other){
    printf("hello!\n");
    Response res;
    res.addr = addr;
    res.msg = "hello back!!\n";
    res.packet_len = *buff_size;
    res.size_of_addr = size_of_addr;
    res.sfd = sfd;
    
    ssp_sendto(res);
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
    

    if (client) {
        printf("I'm a client!\n");
        connection_client(1111);
    }
    else {
        printf("I'm a server!\n");
        connection_server("1111", buffsize, onRecv, onTimeOut, onStdIn, checkExit, onExit, NULL);
    }
        
    return 0;
}

