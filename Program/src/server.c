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
#include <time.h>
#include "port.h"

static int exit_now;
 
 
//if conn_typ == 1, tcp, if 0 udp
int prepareHost(char *port, int conn_type)
{

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_V4MAPPED;

    switch (conn_type) {

        case 0:
            hints.ai_socktype = SOCK_DGRAM;
            break;
        case 1:
            hints.ai_socktype = SOCK_STREAM;
    }
    
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

        if (err == -1)
        {
            perror("set sock opt");
            ssp_close(sfd);
            continue;
        }

        err = bind(sfd, cur->ai_addr, cur->ai_addrlen);
        if (err == -1)
        {
            perror("bind");
            ssp_close(sfd);
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
void connection_server(char* port, int initial_buff_size,
    int (*onRecv)(int sfd, char *packet, uint32_t packet_len,  uint32_t *buff_size, void *addr, size_t size_of_addr, void *other), 
    int (*onTimeOut)(void *other),
    int (*onStdIn)(void *other),
    int (*checkExit)(void *other),
    void (*onExit)(void *other),
    void *other)
{
    int sfd = prepareHost(port, 1);
    int err = listen(sfd, 10);

    if (err == -1)
        ssp_error("listen failed\n");

    size_t size_of_socket_struct[1];
    *size_of_socket_struct = 0;

    void *socket_set = ssp_init_socket_set(size_of_socket_struct);
    void *read_socket_set = ssp_init_socket_set(size_of_socket_struct);

    ssp_fd_zero(socket_set);
    ssp_fd_set(sfd, socket_set);
    ssp_fd_set(STDIN_FILENO, socket_set);

    uint32_t *buff_size = ssp_alloc(1, sizeof(uint32_t));
    checkAlloc(buff_size, 1);

    *buff_size = initial_buff_size + 10;
    uint32_t prev_buff_size = *buff_size;

    char *buff = ssp_alloc(sizeof(char), *buff_size);
    checkAlloc(buff, 1);

    size_t size_of_addr[1];
    *size_of_addr = 0;
    void *addr = ssp_init_sockaddr_struct("", port, size_of_addr);

    for (;;)
    {
        
        memcpy(read_socket_set, socket_set, size_of_socket_struct[0]);
        int nrdy = ssp_select(sfd, read_socket_set, NULL,  NULL, 100e3);

        if (exit_now || checkExit(other)){
            ssp_printf("exiting server thread\n");
            break;
        }
    
        if(!resizeBuff(&buff, buff_size, &prev_buff_size)){
            ssp_printf("packet too large, cannot resize buffer\n");
        }

        if (nrdy == -1) {
            ssp_error("select");
            continue;
        }
        //timeout
        if (nrdy == 0) {
            if (onTimeOut(other) == -1)
                ssp_printf("timeout failed\n");
            continue;
        }

        for(int i = 0; i < FD_SETSIZE; i++) {

            if (ssp_fd_is_set(i, read_socket_set)) {
                ssp_printf("receiving from socket %d!\n", i);

                if (i == STDIN_FILENO) {
                    onStdIn(other);
                    break;
                }

                if (i == sfd) {
                    int new_socket = accept(i, (struct sockaddr*) addr, size_of_addr);
                    if (new_socket < 0)
                        ssp_error ("accept failed");
                    
                    ssp_fd_set(new_socket, socket_set);
                    ssp_fd_set(new_socket, read_socket_set);
                    continue;
                }

                int count = ssp_recvfrom(i, buff, *buff_size, 0, NULL, NULL);
                
                if (count < 0) {
                    ssp_error("recv failed server");
                    ssp_close(i);
                    ssp_fd_clr(i, socket_set);
                }
                else if (count >= *buff_size) {   
                    ssp_printf("packet too large %d\n", count);
                }
                else {
                    ssp_printf("reading\n");
                    if (onRecv(i, buff, count, buff_size, addr, size_of_addr[0], other) == -1)
                        ssp_printf("recv failed\n");
                }
            }
        }
    }
    ssp_free(addr);
    ssp_free(read_socket_set);
    ssp_free(socket_set);
    ssp_free(buff_size);
    ssp_free(buff);
    ssp_close(sfd);
    onExit(other);
}




//see header file
void connectionless_server(char* port, int initial_buff_size,
    int (*onRecv)(int sfd, char *packet, uint32_t packet_len,  uint32_t *buff_size, void *addr, size_t size_of_addr, void *other), 
    int (*onTimeOut)(void *other),
    int (*onStdIn)(void *other),
    int (*checkExit)(void *other),
    void (*onExit)(void *other),
    void *other)
{
    int sfd = prepareHost(port, 0);

    size_t size_of_socket_struct[1];
    *size_of_socket_struct = 0;

    void *socket_set = ssp_init_socket_set(size_of_socket_struct);
    void *read_socket_set = ssp_init_socket_set(size_of_socket_struct);

    ssp_fd_zero(socket_set);
    ssp_fd_set(sfd, socket_set);
    ssp_fd_set(STDIN_FILENO, socket_set);

    uint32_t *buff_size = ssp_alloc(1, sizeof(uint32_t));
    checkAlloc(buff_size, 1);

    *buff_size = initial_buff_size + 10;
    uint32_t prev_buff_size = *buff_size;

    char *buff = ssp_alloc(sizeof(char), *buff_size);
    checkAlloc(buff, 1);

    size_t size_of_addr[1];
    *size_of_addr = 0;
    void *addr = ssp_init_sockaddr_struct("", port, size_of_addr);


    for (;;)
    {

        memcpy(read_socket_set, socket_set, size_of_socket_struct[0]);
        int nrdy = ssp_select(FD_SETSIZE, read_socket_set, NULL,  NULL, 100e3);

        if (exit_now || checkExit(other)){
            ssp_printf("exiting server thread\n");
            break;
        }
    
        if(!resizeBuff(&buff, buff_size, &prev_buff_size)){
            ssp_printf("packet too large, cannot resize buffer\n");
        }

        if (nrdy == -1) {
            ssp_error("select");
            continue;
        }
        
        if (nrdy == 0) {
            if (onTimeOut(other) == -1)
                ssp_printf("timeout failed\n");
            continue;
        }
        
        if (ssp_fd_is_set(STDIN_FILENO, read_socket_set)) {
            onStdIn(other);
            continue;
        }

        if (ssp_fd_is_set(sfd, read_socket_set)) {

            int count = ssp_recvfrom(sfd, buff, *buff_size, MSG_TRUNC, addr, size_of_addr[0]);

            if (count == -1) {
                ssp_error("recv failed server");
            }
            else if (count >= *buff_size) {   
                ssp_printf("packet too large\n");
            }
            else {
                if (onRecv(sfd, buff, count, buff_size, addr, size_of_addr[0], other) == -1)
                    ssp_printf("recv failed\n");
            }
        }
    }
    ssp_free(addr);
    ssp_free(read_socket_set);
    ssp_free(socket_set);
    ssp_free(buff_size);
    ssp_free(buff);
    ssp_close(sfd);
    onExit(other);
}




//https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpclient.c
void connectionless_client(char *hostname, char*port, int packet_len, void *onSendParams, void *onRecvParams, void *checkExitParams, void *onExitParams,
    int (*onSend)(int sfd, struct sockaddr_in client, void *onSendParams),
    int (*onRecv)(int sfd, char *packet, uint32_t packet_len, uint32_t *buff_size, void *addr, size_t size_of_addr, void *onRecvParams) ,
    int (*checkExit)(void *checkExitParams),
    void (*onExit)(void *params))
{

    int sfd, count, port_val;
    socklen_t serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    uint32_t *buff_size = ssp_alloc(1, sizeof(uint32_t));
    checkAlloc(buff_size, 1);

    *buff_size = packet_len + 10;

    uint32_t prev_buff_size = *buff_size;

    char *buff = ssp_alloc(sizeof(char), prev_buff_size);
    checkAlloc(buff, 1);

//--------------------------------------------------------------
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
    
    size_t size_of_addr[1];
    *size_of_addr = 0;
    void *addr = ssp_init_sockaddr_struct(hostname, port, size_of_addr);

//--------------------------------------------------------------

    for (;;) {

        if (exit_now || checkExit(checkExitParams))
             break;
        
        if(!resizeBuff(&buff, buff_size, &prev_buff_size)){
            ssp_error("packet too large, cannot resize buffer\n");
        }

        if (onSend(sfd, serveraddr, onSendParams)) 
            ssp_error("send failed\n");

        count = ssp_recvfrom(sfd, buff, packet_len, MSG_DONTWAIT, &serveraddr,  size_of_addr[0]);
       
        if (count == -1)
            continue;

        else if (count >= *buff_size){   
            ssp_error("packet too large\n");
            continue;
        }
        else{
            if (onRecv(sfd, buff, count, buff_size, (void *)&serveraddr, size_of_addr[0], onRecvParams) == -1)
                ssp_error("recv failed\n");
        }
        
    }

    free(buff_size);
    free(buff);
    ssp_close(sfd);
    onExit(onExitParams);
}


//https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpclient.c
void connection_client(char *hostname, char*port, int packet_len, void *onSendParams, void *onRecvParams, void *checkExitParams, void *onExitParams,
    int (*onSend)(int sfd, struct sockaddr_in client, void *onSendParams),
    int (*onRecv)(int sfd, char *packet, uint32_t packet_len, uint32_t *buff_size, void *addr, size_t size_of_addr, void *onRecvParams) ,
    int (*checkExit)(void *checkExitParams),
    void (*onExit)(void *params))
{

    int sfd, count, port_val;
    socklen_t serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    uint32_t *buff_size = ssp_alloc(1, sizeof(uint32_t));
    checkAlloc(buff_size, 1);

    *buff_size = packet_len + 10;

    uint32_t prev_buff_size = *buff_size;

    char *buff = ssp_alloc(sizeof(char), prev_buff_size);
    checkAlloc(buff, 1);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
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

    //size_t size_of_addr = sizeof(struct sockaddr);

    size_t size_of_addr[1];
    *size_of_addr = 0;
    void *addr = ssp_init_sockaddr_struct(hostname, port, size_of_addr);

    
    if (connect(sfd, (struct sockaddr *)&serveraddr, size_of_addr[0]) != 0) { 
        ssp_error("connection with the server failed...\n"); 
        exit_now = 1;
    }

    for (;;) {
        
        if (exit_now || checkExit(checkExitParams))
             break;
        
        if(!resizeBuff(&buff, buff_size, &prev_buff_size)){
            ssp_printf("packet too large, cannot resize buffer\n");
        }

        if (onSend(sfd, serveraddr, onSendParams)) 
            ssp_error("send failed here\n");

        count = ssp_recvfrom(sfd, buff, packet_len, MSG_DONTWAIT, NULL, NULL);
       
        if (count == -1)
            continue;

        else if (count >= *buff_size){   
            ssp_error("packet too large\n");
            continue;
        }
        else{
            if (onRecv(sfd, buff, count, buff_size, (void *)&serveraddr, size_of_addr[0], onRecvParams) == -1)
                ssp_error("recv failed\n");
        }
        
    }

    printf("exiting loop\n");
    free(buff_size);
    free(buff);
    ssp_close(sfd);
    onExit(onExitParams);
}