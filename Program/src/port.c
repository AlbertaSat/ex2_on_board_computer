
#include "packet.h"
#include "server.h"
#include "port.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mib.h"
#include "filesystem_funcs.h"

#ifdef POSIX_PORT
       #include <pthread.h>
       #include <sys/socket.h>
       #include <stdio.h>
       #include <errno.h>
       #include <limits.h>
       #include <stdio.h>
       #include <stdarg.h>
       #include <unistd.h>

#endif
#include "protocol_handler.h"

int ssp_rename(const char *old, const char *new) {
    #ifdef POSIX_PORT
        return rename(old, new);
    #endif
}

int ssp_write(int fd, const void *buf, size_t count) {
    #ifdef POSIX_PORT
        return write(fd, buf, count);
    #endif
}


int ssp_read(int fd, char* buff, size_t size) {
    #ifdef POSIX_PORT
        return read(fd, buff, size);
    #endif

}

//SEEK_END 2  SEEK_CUR 1  SEEK_SET 0 
int ssp_lseek(int fd, int offset, int whence) {
    #ifdef POSIX_PORT
        return lseek(fd, offset, whence);
    #endif
} 

void *ssp_alloc(uint32_t n_memb, size_t size) {
    #ifdef POSIX_PORT
        return calloc(n_memb, size);
    #endif
}

int ssp_open(char *pathname, int flags) {
    #ifdef POSIX_PORT
        //open with read and write permissions
        return open(pathname, flags, 0666);
    #endif
}

int ssp_close(int fd) {
    #ifdef POSIX_PORT
        return close(fd);
    #endif

}

void ssp_printf( char *stuff, ...) {
    #ifdef POSIX_PORT
        va_list args;
        va_start(args, stuff);
        vfprintf(stdout, stuff, args);
        va_end (args);
        fflush(stdout);
    #endif
}

void ssp_error(char *error){
    #ifdef POSIX_PORT
        perror(error);
    #endif
}
//size is the number of bytes we want to print
void ssp_print_hex(char *stuff, uint32_t size){
    
    uint32_t current_packet_index = 0;
    ssp_printf("printing number of bytes: %u\n", size);

        for (int j = 0; j < size; j += 1) {
            ssp_printf("%x.", 
            stuff[current_packet_index]);
            current_packet_index += 1; 
        }
        ssp_printf("\n");
}   


void ssp_thread_cancel(void *thread_handle) {
    #ifdef POSIX_PORT
    pthread_t * handle = (pthread_t*) thread_handle;
    #endif
    pthread_cancel(*handle);

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
static int on_recv_server(int sfd, char *packet, uint32_t *buff_size, struct sockaddr_storage addr, void *other) {


    Protocol_state *p_state = (Protocol_state *) other;

    #ifdef POSIX_PORT    
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;
    memcpy(p_state->current_server_request->res.addr, posix_client, sizeof(struct sockaddr_in));

    #endif
    p_state->current_server_request->res.sfd = sfd;
    p_state->current_server_request->res.packet_len = p_state->packet_size;

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = p_state->packet_size;

    parse_packet_server(packet, res.packet_len, res, p_state->current_server_request, p_state);
    return 0;

}

static int on_recv_client(int sfd, char *packet, uint32_t *buff_size, struct sockaddr_in addr, void *other) {
    
    #ifdef POSIX_PORT
    struct sockaddr_in* posix_client = (struct sockaddr_in*) &addr;
    #endif

    Client *client = (Client *) other;

    Response res;
    res.addr = posix_client;
    res.sfd = sfd;
    res.packet_len = *buff_size;
    res.msg = client->req->buff;
    
    parse_packet_client(packet, res, client->req, client);
    return 0;
    
}

static int on_send_client(int sfd, struct sockaddr_in addr, void *other) {


    #ifdef POSIX_PORT
    struct sockaddr_in* client_addr = (struct sockaddr_in*) &addr;
    #endif
    
    Response res;    
    Client *client = (Client *) other;
    res.sfd = sfd;
    res.packet_len = client->packet_len;
    res.msg = client->req->buff;
    res.addr = client_addr;

    user_request_handler(res, client->req, client);
    return 0;
  

}

//this function is a callback when using  my posix ports
static int on_time_out_posix(void *other) {

    Protocol_state *p_state = (Protocol_state*) other;
    if(p_state->current_server_request->transaction_sequence_number == 0)
        return 0;

    Response res = p_state->current_server_request->res;
    on_server_time_out(res, p_state->current_server_request, p_state); 
    
    return 0;
}


//this function is just for posix fun
static int on_stdin(void *other) {

    /*
    Protocol_state *p_state = (Protocol_state *) other;
    Request *req = p_state->newClient->req;

    char input[MAX_PATH];
    fgets(input, MAX_PATH, stdin);
    input[strlen(input)-1]='\0';
    
    if (req->type == none){
        if (strnlen(req->source_file_name, MAX_PATH) == 0){
            if (get_file_size(input) == -1){
                ssp_printf("file: %s, we had trouble opening this file, please enter a new file\n", input);
                return 0;
            }
            memcpy(p_state->newClient->req->source_file_name, input, MAX_PATH);
            ssp_printf("Enter a destination file name:\n");
        }
        else if (strnlen(req->destination_file_name, MAX_PATH) == 0){
            memcpy(p_state->newClient->req->destination_file_name, input, MAX_PATH);
            ssp_printf("sending file: %s As file named: %s To cfid enditity %d\n", p_state->newClient->req->source_file_name, p_state->newClient->req->destination_file_name, p_state->newClient->cfdp_id);
            ssp_printf("cancel connection mode (yes):\n");
        } 
        else if (strncmp(input, "yes", 3) == 0){
            ssp_printf("sending file connectionless\n");
            put_request(p_state->newClient->req->source_file_name, p_state->newClient->req->destination_file_name, 0, 0, 0, 1, NULL, NULL, p_state->newClient, p_state);
        } 
        else {
            ssp_printf("sending file connected\n");
            put_request(p_state->newClient->req->source_file_name, p_state->newClient->req->destination_file_name, 0, 0, 0, 0, NULL, NULL, p_state->newClient, p_state); 
        }
   }
   */
    return 0;

}


void *ssp_connectionless_server_task(void *params) {
    
    Protocol_state* p_state = (Protocol_state*) params;
    p_state->transaction_sequence_number = 1;

    #ifdef POSIX_PORT
        udpSelectServer(p_state->server_port, PACKET_LEN, on_recv_server, on_time_out_posix, on_stdin, p_state);
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

    strncpy ((char*)state->server_port, port, 10);


    state->request_list = linked_list();

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

    state->current_server_request = init_request(state->packet_size);
    state->server_handle = handler;
    state->server_thread_attributes = attr;


    return state;

    #endif
}

    
void *ssp_connectionless_client_task(void* params){

    Client *client = (Client *) params;

    char host_name[INET_ADDRSTRLEN];
    char port[10];
    //convert int to char *
    snprintf(port, 10, "%d", client->unitdata_port);

    //convert uint id to char *
    inet_ntop(AF_INET, &client->unitdata_id, host_name, INET_ADDRSTRLEN);

    udpClient(host_name, port, PACKET_LEN, client, client, on_send_client, on_recv_client);
    
    return NULL;
}



Client *ssp_connectionless_client(uint32_t cfdp_id, Protocol_state *p_state) {
    #ifdef POSIX_PORT

    Client *client = calloc(sizeof(Client), 1);
    checkAlloc(client, 1);

    client->req = init_request(PACKET_LEN);

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

    //TODO clean this up, we don't need multiple instances of UT_ports etc
    client->unitdata_port = remote->UT_port;
    client->unitdata_id = remote->UT_address;
    client->mib_info = remote;
    client->pdu_header = get_header_from_mib(p_state->mib, cfdp_id, p_state->my_cfdp_id);

    client->p_state = p_state;

    err = pthread_create(handler, attr, ssp_connectionless_client_task, client);       
    if (0 != err)
        perror("ERROR pthread_create");


    return client;
    #endif

}

void reset_request(Request *req){
    memset(req->source_file_name, 0, MAX_PATH);
    memset(req->destination_file_name, 0, MAX_PATH);
    memset(req->buff, 0, req->buff_len);
    memset(req->res.addr, 0, sizeof(struct sockaddr_in));
    free_file(req->file);
    
    req->type = none;
    
}

Request *init_request(uint32_t buff_len) {

    Request *req = calloc(1, sizeof(Request));

    req->source_file_name = ssp_alloc(MAX_PATH, sizeof(char));
    checkAlloc(req->source_file_name, 1);

    req->destination_file_name = ssp_alloc(MAX_PATH, sizeof(char));
    checkAlloc(req->destination_file_name,  1);


    req->file = NULL;
    req->buff_len = buff_len;
    req->buff = ssp_alloc(buff_len, sizeof(char));
    
    req->res.addr = ssp_alloc(1, sizeof(struct sockaddr_in));
    req->type = none;
    checkAlloc(req->buff,  1);
    return req;
}


void ssp_cleanup_pdu_header(Pdu_header *pdu_header) {
    ssp_free(pdu_header->destination_id);
    ssp_free(pdu_header->source_id);
    ssp_free(pdu_header);
}

void ssp_cleanup_req(Request *req) {
    if (req->file != NULL)
        free_file(req->file);

    if (req->pdu_header != NULL)
        ssp_cleanup_pdu_header(req->pdu_header);

    ssp_free(req->res.addr);
    ssp_free(req->source_file_name);
    ssp_free(req->destination_file_name);
    ssp_free(req->buff);
    ssp_free(req);
}



void ssp_cleanup(Protocol_state *p_state) {

    #ifdef POSIX_PORT
    pthread_t * handle = (pthread_t*) p_state->server_handle;        
    pthread_join(*handle, NULL);

    p_state->request_list->free( p_state->request_list, ssp_cleanup_req);
    ssp_cleanup_req(p_state->current_server_request);
    free_mib(p_state->mib);
    ssp_free(p_state->server_handle);
    ssp_free(p_state->server_port);
    ssp_free(p_state->server_thread_attributes);
    ssp_free(p_state);

    #endif

}


void ssp_cleanup_client(Client *client) {

    
    #ifdef POSIX_PORT
        pthread_t * handle = (pthread_t*) client->client_handle;
        pthread_join(*handle, NULL);
    #endif
    
    ssp_cleanup_req(client->req);
    ssp_free(client->client_handle);
    ssp_free(client->client_thread_attributes);
    ssp_cleanup_pdu_header(client->pdu_header);
    
    free(client);

}


void ssp_free(void *pointer) {
    #ifdef POSIX_PORT
        free(pointer);
    #endif
}