
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
       #include <stdio.h>
       #include <stdarg.h>

#endif

#include "protocol_handler.h"



void ssp_printf(char*stuff, ...) {
    #ifdef POSIX_PORT
        va_list args;
        va_start(args, stuff);
        vfprintf(stdout, stuff, args);
        va_end (args);
    #endif
}


void ssp_error(char *msg) {
    
    #ifdef POSIX_PORT
        perror(msg);
    #endif

}

void ssp_sendto(Response res) {

    #ifdef POSIX_PORT
        struct sockaddr* addr = (struct sockaddr*) res.addr;
        int n = sendto(res.sfd, res.msg, res.packet_len, 0, addr, sizeof(*addr));
        if (n < 0) 
            ssp_error("ERROR in ssp_sendto");
    #endif
}


//this function is a callback when using my posix port
static int on_recv_server(int sfd, char *msg, struct sockaddr_storage addr, void *other) {

    #ifdef POSIX_PORT

    Protocol_state *p_state = (Protocol_state *) other;
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = p_state->packet_size;

    ssp_printf("Server received: %s\n", msg);
    //printf("addr %d port%d\n",ntohl(((struct in_addr*) posix_client)->s_addr), ntohs(posix_client->sin_port));

    packet_handler_server(res, p_state);
    return 0;

    #endif
}

static int on_recv_client(int sfd, char *msg, uint32_t *buff_size, struct sockaddr_in addr, void *other) {
    
    #ifdef POSIX_PORT

    Protocol_state *p_state = (Protocol_state *) other;
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = p_state->packet_size;

    ssp_printf("Client received: %s\n", msg);

    packet_handler_client(res, p_state);
    return 0;
    
    #endif
}

static int on_send_client(int sfd, struct sockaddr_in addr, void *other) {


    #ifdef POSIX_PORT
    Protocol_state *p_state = (Protocol_state *) other;
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = p_state->packet_size;
    res.msg = "some shit\n";

    ssp_sendto(res);

    return 0;
    #endif

}

//this function is a callback when using  my posix ports
static  int on_time_out_posix(void *other) {
    return 0;
}


void *ssp_connectionless_server_task(void *params) {
    
    Protocol_state* state = (Protocol_state*) params;

    #ifdef POSIX_PORT
        udpSelectServer(state->server_port, PACKET_LEN, on_recv_server, on_time_out_posix, state);
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

    
void *ssp_connectionless_client_task(void* params){

    Protocol_state *p_state = (Protocol_state *) params;
    
    // TODO find a client function p_state->client_list->find()
    udpClient(p_state->client->host_name, p_state->client->client_port, PACKET_LEN, p_state, p_state, on_send_client, on_recv_client);
    return NULL;
}



Client *ssp_connectionless_client(char *host_name, char *port, Protocol_state *p_state) {
    #ifdef POSIX_PORT
    int port_len = strnlen(port, 100) + 1;
    int host_len = strnlen(host_name, 100) + 1;

    Client *client = calloc(sizeof(Client), 1);
    checkAlloc(client, 1);

    client->host_name = calloc(sizeof(char), host_len);
    checkAlloc(client->host_name, 1);

    client->client_port = calloc(sizeof(char), port_len);
    checkAlloc(client->client_port, 1);

    pthread_t *handler = calloc(sizeof(pthread_t), 1);
    checkAlloc(handler, 1);

    pthread_attr_t *attr = calloc(sizeof(pthread_attr_t), 1); 
    checkAlloc(attr, 1);

    strncpy(client->client_port, port, port_len);
    strncpy(client->host_name, host_name, host_len);

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

    client->client_handle = handler;
    client->client_thread_attributes = attr;

    //add client to a new list of currently active clients
    p_state->client_list->add(p_state->client_list, client);
    
    p_state->client = client;
    err = pthread_create(handler, attr, ssp_connectionless_client_task, p_state);       
    if (0 != err)
        perror("ERROR pthread_create");


    return client;
    #endif

}


void ssp_cleanup(Protocol_state *p_state) {

    #ifdef POSIX_PORT
    pthread_t * handle = (pthread_t*) p_state->server_handle;        
    pthread_join(*handle, NULL);

    free(p_state->server_handle);
    free(p_state->server_port);
    free(p_state->server_thread_attributes);
    free(p_state);

    #endif

}


void ssp_cleanup_client(Client *client) {

    #ifdef POSIX_PORT
    pthread_t * handle = (pthread_t*) client->client_handle;

    pthread_join(*handle, NULL);
    free(client->host_name);
    free(client->client_handle);
    free(client->client_port);
    free(client->client_thread_attributes);
    free(client);
    #endif


}