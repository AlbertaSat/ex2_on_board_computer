
#include "server_tests.h"
#include <stdio.h>
#include "server.h"
#include "port.h"
#include "tasks.h"


static int onRecv(int sfd, char *packet, uint32_t packet_len,  uint32_t *buff_size, void *addr, size_t size_of_addr, void *other){
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

static int onTimeOut(void *other) {
    return 0;
}
static int onStdIn(void *other){
    return 0;
}
static int checkExit(void *other) {
    return 0;
}
static void onExit(void *other) {
}

//client stuff
static int onSend(int sfd, struct sockaddr_in addr, void *onSendParams) {
    Response res;
    res.addr = &addr;
    res.msg = "hello serer!!\n";
    res.packet_len = 12;
    res.size_of_addr = sizeof(struct sockaddr_in);
    res.sfd = sfd;
    printf("sending msg\n");
    ssp_sendto(res);
    return 0;
}
static int onRecvClient(int sfd, char *packet, uint32_t packet_len, uint32_t *buff_size, void *addr, size_t size_of_addr, void *onRecvParams) {
    printf("recv %s\n", packet);
    return 0;

}

static int checkExitClient(void *params) {
    return 0;
}

static void onExitClient(void *params) {

}

int server_tests(int client){

    int buffsize = 10000;
    char buff[buffsize];
    

    if (client) {
        printf("I'm a client!\n");
        //connection_client(1111);
        connectioned_client("127.0.0.1", "1111", buffsize, NULL, NULL, NULL, NULL, onSend, onRecvClient, checkExitClient, onExitClient);


    }
    else {
        printf("I'm a server!\n");
        connection_server("1111", buffsize, onRecv, onTimeOut, onStdIn, checkExit, onExit, NULL);
    }
        
    return 0;
}

