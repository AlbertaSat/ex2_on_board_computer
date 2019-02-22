
#include "packet.h"
#include "server.h"
#include "port.h"

#include <stdlib.h>
#include <string.h>
#include "utils.h"

#ifdef POSIX_PORT
       #include <pthread.h>
       #include <sys/socket.h>
       #include <stdio.h>
       #include <errno.h>
       #include <limits.h>
#endif


void ssp_error(char *msg) {
    
    #ifdef POSIX_PORT
        perror(msg);
    #endif

}

void ssp_sendto(Client client) {

    #ifdef POSIX_PORT
        struct sockaddr* addr = (struct sockaddr*) client.addr;
        int n = sendto(client.sfd, client.msg, client.packet_len, 0, addr, sizeof(*addr));
        if (n < 0) 
            ssp_error("ERROR in ssp_sendto");
    #endif
}


//this function is a callback when using my silly posix port
static int on_recv_posix(int sfd, char *msg, struct sockaddr_storage addr, void *other) {

    Protocol_state *state = (Protocol_state *) other;
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;

    Client new_client;
    new_client.msg = "stuffffff";
    new_client.addr = posix_client;
    new_client.sfd = sfd;
    new_client.packet_len = state->packet_size;

    printf("msg recv: %s\n", msg);
    printf("addr %d port%d\n",ntohl(((struct in_addr*) posix_client)->s_addr), ntohs(posix_client->sin_port));
    ssp_sendto(new_client);


    return 1;
}

//this function is a callback when using  my silly posix ports
static  int on_time_out_posix(void *other) {
    return 1;
}



void *ssp_connectionless_server_task(void *params) {
    Protocol_state* state = (Protocol_state*) params;

    #ifdef POSIX_PORT
        udpSelectServer(state->server_port, PACKET_LEN, on_recv_posix, on_time_out_posix, state);
    #endif

    return NULL;
}


Protocol_state* ssp_connectionless_server(char *port) {
    #ifdef POSIX_PORT
        Protocol_state *state = calloc(sizeof(Protocol_state), 1);
        state->packet_size = PACKET_LEN;

        pthread_t *handler = calloc(sizeof(pthread_t), 1);
        checkAlloc(handler, 1);

        pthread_attr_t *attr = calloc(sizeof(pthread_attr_t), 1); 
        checkAlloc(attr, 1);

        state->server_port = calloc(sizeof(char), 10);
        checkAlloc(state->server_port, 1);

        strncpy (state->server_port, port, 10);

        int err = pthread_attr_init(attr);
        if (0 != err) 
            perror("pthread_init failed");


        size_t stack_size = STACK_ALLOCATION;
        err = pthread_attr_setstacksize(attr, stack_size);

        if (0 != err)
            perror("ERROR pthread_attr_setstacksize %d");

        if (EINVAL == err) {
            printf("the stack size is less that PTHREAD_STACK_MIN %d\n", PTHREAD_STACK_MIN);
        }

        err = pthread_create(handler, attr, ssp_connectionless_server_task, state);       
        if (0 != err)
            perror("ERROR pthread_create");


        state->server_handle = handler;
        state->server_thread_attributes = attr;

        return state;

    #endif
}


void ssp_connectionless_client(char *host_name, char*port) {
    #ifdef POSIX_PORT
        udpClient(host_name, port);
    #endif

}

void ssp_cleanup(Protocol_state *state) {

    #ifdef POSIX_PORT
        pthread_t * handle = (pthread_t*) state->server_handle;

        pthread_join(*handle, NULL);

        free(state->server_handle);
        free(state->server_port);
        free(state->server_thread_attributes);
        free(state);

    #endif

}