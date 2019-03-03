/*------------------------------------------------------------------------------
CMPT-361-AS50(1) - 2017 Fall - Introduction to Networks
Assignment #1
Evan Giese 1689223

This file is the header file for server.c 
------------------------------------------------------------------------------*/
#ifndef SERVER_H
#define SERVER_H

//figure out what to inlcude for types ask nick
#include <netinet/in.h>

/*------------------------------------------------------------------------------
    Purpose: This function creates a host server on the specified port
             on localhost
    Perameters: char *port is the port number you want to run the host on
    Return: It returns a socket descriptor to a UDP ready port
------------------------------------------------------------------------------*/
int prepareUdpHost(char *port);

/*------------------------------------------------------------------------------
    Purpose:    This function creates a udp select server on the socket sfd.
    Perameters: int sfd: The socket descriptor created by prepareUdpHost
                void *other: this is the void* that we want to pass into 
                onRecv, and onTimeout
    Return:     None
------------------------------------------------------------------------------*/
/*-----------------------------CALLBACK onRecv----------------------------------
    Purpose:    This function handles what happends when a user receives a 
                packet
    Perameters: int sfd: Is the socket descriptor that the msg was received on
                this is useful for resending messges.
                char *msg: Is the packet that was sent
                struct sockaddr_storage client: Is the address of the sender
                void *other: Is the void * we passed into selectUdp
    Return:     None
------------------------------------------------------------------------------*/
/*-----------------------------CALLBACK onTimeOut-------------------------------
    Purpose:    This function 
    Perameters: void *other: Is the void * we passed into selectUdp
    Return:     None
------------------------------------------------------------------------------*/

void udpSelectServer(char *port, int packet_len,
    int (*onRecv)(int sfd, char *msg, struct sockaddr_storage client, void *other), 
    int (*onTimeOut)(void *other),
    void *other);

/*-----------------------------CALLBACK onTimeOut-------------------------------
    Purpose:    This is a simple udp client 
    Perameters: hostname is the name of an address eg, 127.0.0.1, port is the por
                channel_size is the size of the channel in bytes
    eg, 1111
    Return:     None
------------------------------------------------------------------------------*/


int *prepareSignalHandler(void);


void udpClient(char *hostname, char*port, int packet_len, void *onSendParams, void *onRecvParams, 
    int (*onSend)(int sfd, struct sockaddr_in client, void *onSendParams),
    int (*onRecv)(int sfd, char *msg, uint32_t *buff_size, struct sockaddr_in client, void *onRecvParams));

    
#endif //SERVER_H
