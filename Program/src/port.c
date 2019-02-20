
#include "packet.h"
#include "server.h"
#include "port.h"
#include "stdio.h"
#include <stdlib.h>
#include <sys/socket.h>


int on_recv_posix(int sfd, char *msg, struct sockaddr_storage client, void *other) {
    
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &client;
    int address_type = posix_client->sin_port;

    printf("%d\n", address_type);
    printf("%s\n", (char *) other);
    printf("%d\n", sfd);
    printf("%s\n", msg);
    return 1;
}

int on_time_out_posix(void *other) {
    printf("%s\n", (char *) other);
    printf("timeout\n");
    return 1;
}


void ss_connectionless_server(char *port) {

    #ifdef POSIX_PORT
        //for exiting nicely, optional
        PDU_header *stuff = malloc(sizeof(PDU_header));

        int sfd = prepareUdpHost(port);
        selectUdp(sfd, stuff, on_recv_posix, on_time_out_posix);
    #endif
}
