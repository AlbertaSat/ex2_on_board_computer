


#include "protocol_handler.h"
#include "tasks.h"
#include "server.h"
#include "port.h"
#include <string.h>
#include "mib.h"
#include "filesystem_funcs.h"

/*------------------------------------------------------------------------------
    
    Callbacks for the tasks bellow

------------------------------------------------------------------------------*/
//this function is a callback when using my posix port
static int on_recv_server(int sfd, char *packet, uint32_t *buff_size, void *addr, void *other) {

    Protocol_state *p_state = (Protocol_state *) other;
    Response res;
    res.addr = addr;
    res.sfd = sfd;
    res.packet_len = p_state->packet_size;
    p_state->current_server_request->res = res;

    parse_packet_server(packet, res.packet_len, res, p_state->current_server_request, p_state);
    return 0;

}

static int on_recv_client(int sfd, char *packet, uint32_t *buff_size, void *addr, void *other) {
    

    Client *client = (Client *) other;
    if (client->req == NULL)
        return 0;

    Response res;
    res.addr = addr;
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
    if (client->req == NULL)
        return 0;

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


/*------------------------------------------------------------------------------
    
    Tasks

------------------------------------------------------------------------------*/
void *ssp_connectionless_server_task(void *params) {
    
    Protocol_state* p_state = (Protocol_state*) params;
    p_state->transaction_sequence_number = 1;

    #ifdef POSIX_PORT
        udpSelectServer(p_state->server_port, PACKET_LEN, on_recv_server, on_time_out_posix, on_stdin, p_state);
    #endif

    return NULL;
}


void ssp_connectionless_server(Protocol_state *p_state) {
    /*
    Protocol_state *state = ssp_alloc(sizeof(Protocol_state), 1);
    state->packet_size = PACKET_LEN;

    state->server_port = ssp_alloc(sizeof(char), 10);
    checkAlloc(state->server_port, 1);
    strncpy ((char*)state->server_port, port, 10);

    state->request_list = linked_list();
    state->current_server_request = init_request(state->packet_size);
    */
    p_state->server_handle = ssp_thread_create(STACK_ALLOCATION, ssp_connectionless_server_task, p_state);
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


    Client *client = ssp_alloc(sizeof(Client), 1);
    checkAlloc(client, 1);

    client->req = NULL;
    client->req_queue = linked_list();
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

    client->client_handle = ssp_thread_create(STACK_ALLOCATION, ssp_connectionless_client_task, client);
    return client;
}

/*------------------------------------------------------------------------------
    
    free functions

------------------------------------------------------------------------------*/




void ssp_cleanup_p_state(Protocol_state *p_state) {

    ssp_thread_join(p_state->server_handle);
    p_state->request_list->free(p_state->request_list, ssp_cleanup_req);
    ssp_cleanup_req(p_state->current_server_request);
    free_mib(p_state->mib);
    ssp_free(p_state->server_handle);
    ssp_free(p_state->server_port);
    ssp_free(p_state);

}


void ssp_cleanup_client(Client *client) {

    ssp_thread_join(client->client_handle);
    client->req_queue->free(client->req_queue, ssp_cleanup_req);
    ssp_cleanup_req(client->req);
    ssp_free(client->client_handle);
    ssp_cleanup_pdu_header(client->pdu_header);
    ssp_free(client);
}
