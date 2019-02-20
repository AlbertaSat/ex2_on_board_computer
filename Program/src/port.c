
#include "packet.h"
#include "server.h"
#include "port.h"
#include "stdio.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>


void ss_error(char *msg) {
    
    #ifdef POSIX_PORT
        perror(msg);
    #endif

}

void ss_sendto(Client client) {

    #ifdef POSIX_PORT
        struct sockaddr* addr = (struct sockaddr*) client.addr;
        printf("msg to send: %s\n", client.msg);
        printf("file descriptor:%d\n", client.sfd);
        
        int n = sendto(client.sfd, client.msg, strnlen(client.msg, 100), 0, addr, sizeof(*addr));
        if (n < 0) 
            ss_error("ERROR in ss_sendto");
    #endif
}



//this function is a callback when using my silly posix port
static int on_recv_posix(int sfd, char *msg, struct sockaddr_storage addr, void *other) {
    
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;
    Client new_client;
    new_client.msg = "stuffffff";
    new_client.addr = posix_client;
    new_client.sfd = sfd;
    printf("msg recv: %s\n", msg);
    ss_sendto(new_client);
    return 1;
}

//this function is a callback when using  my silly posix ports
static  int on_time_out_posix(void *other) {
    //printf("%s\n", (char *) other);
    //printf("timeout\n");
    return 1;
}


void ss_connectionless_server(char *port) {

    #ifdef POSIX_PORT
        //for exiting nicely, optional
        PDU_header *stuff = malloc(sizeof(PDU_header));
        int sfd = prepareUdpHost(port);
        selectUdp(sfd, PACKET_LEN, on_recv_posix, on_time_out_posix, stuff);
    #endif
}
