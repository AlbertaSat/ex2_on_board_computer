/*------------------------------------------------------------------------------
CMPT-361-AS50(1) - 2017 Fall - Introduction to Networks
Assignment #2
Evan Giese 1689223

This is my file for server.c. It develops a udp server for select.
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <libgen.h>
#include <sys/select.h>
#include <sys/time.h>
#include "utils.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

static int exit_now;


//this code is reused from assignment 1, with small changes
//see header file
int prepareUdpHost(char *port)
{

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_V4MAPPED;
    hints.ai_socktype = SOCK_DGRAM;

    int err = getaddrinfo(NULL, port, &hints, &res);

    if (err != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

    int sfd;
    struct addrinfo *cur;

    for (cur = res; cur != NULL; cur = cur->ai_next)
    {
        sfd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);

        if (sfd < 0)
        {
            perror("socket");
        }

        int val = 1;
        err = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
        err = bind(sfd, cur->ai_addr, cur->ai_addrlen);

        if (err == -1)
        {
            perror("bind");
            close(sfd);
            continue;
        }

        if (err == -1)
        {
            perror("listen");
            close(sfd);
            continue;
        }

        freeaddrinfo(cur);
        break;
    }

    if (cur == NULL)
    {
        fprintf(stderr, "could not create server\n");
        exit(EXIT_FAILURE);
    }

    return sfd;
}
/*------------------------------------------------------------------------------
    This function is the inturpt handler for sigaction, right now i just 
    handle sigint, so we can exit nicely.
------------------------------------------------------------------------------*/

static void interuptHandler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        exit_now = 1;
        break;
    }
}


//see header file
int *prepareSignalHandler()
{
    struct sigaction actionData;
    sigemptyset(&actionData.sa_mask);
    actionData.sa_handler = interuptHandler;
    actionData.sa_flags = 0;

    if (sigaction(SIGINT, &actionData, NULL) == -1)
    {
        perror("sigaction sigint failed\n");
        exit(EXIT_FAILURE);
    }
    return &exit_now;
}

static int resizeBuff(char **buffer, uint32_t *newBufferSize, uint32_t *prev_buff_size) {

     if (*newBufferSize != *prev_buff_size) {
            *buffer = realloc(*buffer, *newBufferSize);

            if(checkAlloc(*buffer, 0)){
                *prev_buff_size = *newBufferSize;
                return 1;
            } 
            return 0;
    }
    return 1;
}

//see header file
void udpSelectServer(char* port, int packet_len,
    int (*onRecv)(int sfd, char *packet, uint32_t packet_len,  uint32_t *buff_size, void *addr, size_t size_of_addr, void *other), 
    int (*onTimeOut)(void *other),
    int (*onStdIn)(void *other),
    int (*checkExit)(void *other),
    void (*onExit)(void *other),
    void *other)
{
    int sfd = prepareUdpHost(port);

    fd_set masterReadFds;
    FD_ZERO(&masterReadFds);
    FD_SET(STDIN_FILENO, &masterReadFds);
    FD_SET(sfd, &masterReadFds);

    uint32_t *buff_size = calloc(1, sizeof(uint32_t));
    checkAlloc(buff_size, 1);


    *buff_size = packet_len + 10;

    uint32_t prev_buff_size = *buff_size;

    char *buff = calloc(sizeof(char), *buff_size);
    checkAlloc(buff, 1);

    struct sockaddr_storage *client;
    client = calloc(sizeof(struct sockaddr_storage), 1);
    checkAlloc(client, 1);


    for (;;)
    {
        struct timeval timeout = {
            .tv_sec = 0,
            .tv_usec = 1000e3,
        };

        fd_set readFds = masterReadFds;
        int nrdy = select(sfd + 1, &readFds, NULL, NULL, &timeout);


        if (exit_now || checkExit(other)){
            printf("exiting server thread\n");
            break;
        }
    
        if(!resizeBuff(&buff, buff_size, &prev_buff_size)){
            printf("packet too large, cannot resize buffer\n");
        }

        if (nrdy == -1) {
            perror("select");
            continue;
        }
        //timeout
        if (nrdy == 0) {
            if (onTimeOut(other) == -1)
                printf("timeout failed\n");
            continue;
        }
        
        if (FD_ISSET(STDIN_FILENO, &readFds)) {
            onStdIn(other);
            continue;
        }

        //http://www.microhowto.info/howto/listen_for_and_receive_udp_datagrams_in_c.html
        // good article!
        if (FD_ISSET(sfd, &readFds))
        {
            
            socklen_t client_len = sizeof(*client);
            int count = recvfrom(sfd, buff, packet_len, 0, (void *) client, &client_len);
            

            if (count == -1)
            {
                perror("recv failed");
            }
            else if (count >= *buff_size)
            {   
                printf("packet too large\n");
                continue;
            }
            else
            {
                if (onRecv(sfd, buff, count, buff_size, (void*) client, sizeof(struct sockaddr), other) == -1)
                    printf("recv failed\n");
            }
        }
    }
    free(buff_size);
    free(client);
    free(buff);
    close(sfd);
    onExit(other);
}




//https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpclient.c
void udpClient(char *hostname, char*port, int packet_len, void *onSendParams, void *onRecvParams, void *checkExitParams, void *onExitParams,
    int (*onSend)(int sfd, struct sockaddr_in client, void *onSendParams),
    int (*onRecv)(int sfd, char *packet, uint32_t packet_len, uint32_t *buff_size, void *addr, size_t size_of_addr, void *onRecvParams) ,
    int (*checkExit)(void *checkExitParams),
    void (*onExit)(void *params))
{

    int sfd, count, port_val;
    socklen_t serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    uint32_t *buff_size = malloc(sizeof(uint32_t));
    checkAlloc(buff_size, 1);

    *buff_size = packet_len + 10;

    uint32_t prev_buff_size = *buff_size;

    char *buff = calloc(sizeof(char), prev_buff_size);
    checkAlloc(buff, 1);

    prepareSignalHandler();

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0) 
        perror("ERROR opening socket");

    port_val = atoi(port);

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    bcopy((char *)server->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(port_val);
    serverlen = sizeof(serveraddr);
        
    for (;;) {

        if (exit_now || checkExit(checkExitParams))
             break;
        
        if(!resizeBuff(&buff, buff_size, &prev_buff_size)){
            printf("packet too large, cannot resize buffer\n");
        }

        if (onSend(sfd, serveraddr, onSendParams)) 
            printf("send failed\n");


        count = recvfrom(sfd, buff, packet_len, MSG_DONTWAIT, (struct sockaddr*)&serveraddr, &serverlen);

        if (count == -1)
        {
            //perror("recv failed");
        }
        else if (count >= *buff_size)
        {   
            printf("packet too large\n");
            continue;
        }
        else
        {
            if (onRecv(sfd, buff, count, buff_size, (void *)&serveraddr, sizeof(struct sockaddr), onRecvParams) == -1)
                printf("recv failed\n");
        }
        
    }

    free(buff_size);
    free(buff);
    close(sfd);
    onExit(onExitParams);
}


