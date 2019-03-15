
#include "packet.h"
#include "server.h"
#include "port.h"

#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mib.h"


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

void *ssp_alloc(uint32_t n_memb, size_t size) {
    #ifdef POSIX_PORT
        return calloc(n_memb, size);
    #endif
}


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
static int on_recv_server(int sfd, char *msg, uint32_t *buff_size, struct sockaddr_storage addr, void *other) {


    Protocol_state *p_state = (Protocol_state *) other;

    #ifdef POSIX_PORT    
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;
    #endif

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = p_state->packet_size;

    //filles the request struct
    parse_packet_server(msg, p_state->current_server_request, p_state);

    //ssp_printf("Server received: %s\n", msg);
    packet_handler_server(res, p_state->current_server_request, p_state);
    return 0;

}

static int on_recv_client(int sfd, char *msg, uint32_t *buff_size, struct sockaddr_in addr, void *other) {
    
    #ifdef POSIX_PORT
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;
    #endif

    Protocol_state *p_state = (Protocol_state *) other;

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = *buff_size;

    Client *client = p_state->newClient;

    //filles the request struct
    parse_packet_client(msg, client->incoming_req, client, p_state);

    //ssp_printf("Client received: %s\n", msg);
    packet_handler_client(res, client->incoming_req, client, p_state);
    return 0;
    
}

static int on_send_client(int sfd, struct sockaddr_in addr, void *other) {

    #ifdef POSIX_PORT
    Protocol_state *p_state = (Protocol_state *) other;
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;

    Client *client = p_state->newClient;

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = client->packet_len;

    user_request_handler(res, client->outGoing_req, client, p_state);
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

    state->current_server_request = calloc(1, sizeof(Request));
    state->server_handle = handler;
    state->server_thread_attributes = attr;
    return state;

    #endif
}

    
void *ssp_connectionless_client_task(void* params){

    Protocol_state *p_state = (Protocol_state *) params;

    char host_name[INET_ADDRSTRLEN];
    char port[10];
    //convert int to char *
    snprintf(port, 10, "%d", p_state->newClient->unitdata_port);

    //convert uint id to char *
    inet_ntop(AF_INET, &p_state->newClient->unitdata_id, host_name, INET_ADDRSTRLEN);

    udpClient(host_name, port, PACKET_LEN, p_state, p_state, on_send_client, on_recv_client);
    
    return NULL;
}



Client *ssp_connectionless_client(uint32_t cfdp_id, Protocol_state *p_state) {
    #ifdef POSIX_PORT

    Client *client = calloc(sizeof(Client), 1);
    checkAlloc(client, 1);

    client->outGoing_req = calloc(1, (sizeof(Request)));
    checkAlloc(client->outGoing_req, 1);

    client->incoming_req = calloc(1, (sizeof(Request)));
    checkAlloc(client->incoming_req, 1);

    pthread_t *handler = calloc(sizeof(pthread_t), 1);
    checkAlloc(handler, 1);

    pthread_attr_t *attr = calloc(sizeof(pthread_attr_t), 1); 
    checkAlloc(attr, 1);

    int err = pthread_attr_init(attr);
    if (0 != err) 
        perror("pthread_init failed");

    size_t stack_size = STACK_ALLOCATION;
    err = pthread_attr_setstacksize(attr, stack_size);

    if (0 != err)
        perror("ERROR pthread_attr_setstacksize %d");

    if (EINVAL == err)
        printf("the stack size is less that PTHREAD_STACK_MIN %d\n", PTHREAD_STACK_MIN);

    client->client_handle = handler;
    client->client_thread_attributes = attr;
    client->packet_len = PACKET_LEN;
    client->cfdp_id = cfdp_id;

    List *entity_list = p_state->mib->remote_entities;
    Remote_entity *remote = entity_list->find(entity_list, cfdp_id, NULL, NULL);

    if (remote == NULL)
        ssp_printf("couldn't find entity in Remote_entity list\n");


    client->unitdata_port = remote->UT_port;
    client->unitdata_id = remote->UT_address;
    client->mib_info = remote;

    //building the pdu header here
    client->pdu_header = ssp_alloc(1, sizeof(Pdu_header));
    checkAlloc(client->pdu_header, 1);


    //these will need to be set with a config file
    Pdu_header* header = client->pdu_header;
    header->reserved_bit_0 = 0;
    header->reserved_bit_1 = 0;
    header->reserved_bit_2 = 0;
    header->CRC_flag = client->mib_info->CRC_required;
    header->direction = 1;
    header->PDU_type = 0;
    header->PDU_data_field_len = 1200;
    header->transaction_seq_num_len = 3;
    header->transaction_sequence_number = 0;
    header->length_of_entity_IDs = 1; 

    header->destination_id = ssp_alloc(header->length_of_entity_IDs, sizeof(u_int8_t));
    checkAlloc(header->destination_id, 1);
    memcpy(header->destination_id, &remote->cfdp_id, header->length_of_entity_IDs);


    header->source_id = ssp_alloc(header->length_of_entity_IDs, sizeof(u_int8_t));
    checkAlloc(header->source_id, 1);
    memcpy(header->source_id, &p_state->my_cfdp_id, header->length_of_entity_IDs);


    //TODO lock this
    p_state->newClient = client;

    err = pthread_create(handler, attr, ssp_connectionless_client_task, p_state);       
    if (0 != err)
        perror("ERROR pthread_create");


    return client;
    #endif

}

void ssp_cleanup_req(Request *req) {

    free(req->buff);
    free(req);
}



void ssp_cleanup(Protocol_state *p_state) {

    #ifdef POSIX_PORT
    pthread_t * handle = (pthread_t*) p_state->server_handle;        
    pthread_join(*handle, NULL);

    free_mib(p_state->mib);
    free(p_state->server_handle);
    free(p_state->server_port);
    free(p_state->server_thread_attributes);
    free(p_state->current_server_request);
    free(p_state);

    #endif

}

void ssp_cleanup_client(Client *client) {

    
    #ifdef POSIX_PORT
        pthread_t * handle = (pthread_t*) client->client_handle;
        pthread_join(*handle, NULL);
    #endif
    
    ssp_cleanup_req(client->outGoing_req);
    free(client->incoming_req);
    free(client->client_handle);
    free(client->client_thread_attributes);
    free(client->pdu_header->destination_id);
    free(client->pdu_header->source_id);
    free(client->pdu_header);

    free(client);

}


void ssp_free(void *pointer) {
    #ifdef POSIX_PORT
        free(pointer);
    #endif

}