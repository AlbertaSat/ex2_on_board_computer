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


static int exitNow = 0;
static int new_packet_len;


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
        exitNow = 1;
        break;
    }
}
//see header file
void prepareSignalHandler()
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
}


//see header file
void selectUdp(int sfd, int packet_len,
    int (*onRecv)(int sfd, char *msg, struct sockaddr_storage client, void *other), 
    int (*onTimeOut)(void *other),
    void *other)
{

    new_packet_len = packet_len;

    //prepareSignalHandler();
    fd_set masterReadFds;
    FD_ZERO(&masterReadFds);
    FD_SET(STDIN_FILENO, &masterReadFds);
    FD_SET(sfd, &masterReadFds);
    char *buff = calloc(sizeof(char), new_packet_len);
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

        if (exitNow)
            break;

        if (nrdy == -1)
        {
            perror("select");
            continue;
        }
        //timeout
        if (nrdy == 0)
        {
            if (onTimeOut(other) == -1)
                printf("timeout failed\n");
            continue;
        }

        //http://www.microhowto.info/howto/listen_for_and_receive_udp_datagrams_in_c.html
        // good article!
        if (FD_ISSET(sfd, &readFds))
        {
            
            socklen_t client_len = sizeof(*client);
            int count = recvfrom(sfd, buff, new_packet_len, 0, (struct sockaddr*)client, &client_len);

            if (count == -1)
            {
                perror("recv failed");
            }
            else if (count == sizeof(buff))
            {
                printf("packet too large\n");
                continue;
            }
            else
            {
                if (onRecv(sfd, buff, *client, other) == -1)
                    printf("recv failed\n");
            }
        }
    }
    free(client);
    free(buff);
    close(sfd);
}