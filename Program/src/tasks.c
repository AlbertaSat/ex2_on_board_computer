


#include "protocol_handler.h"
#include "tasks.h"
#include "server.h"
#include "port.h"
#include <string.h>
#include "mib.h"
#include "filesystem_funcs.h"
#include <stdio.h>

/*------------------------------------------------------------------------------
    
    Callbacks for the tasks bellow

------------------------------------------------------------------------------*/
//this function is a callback when using my posix port
static int on_recv_server(int sfd, char *packet, uint32_t *buff_size, void *addr, size_t size_of_addr, void *other) {

    Protocol_state *p_state = (Protocol_state *) other;
    Response res;
    res.addr = addr;
    res.sfd = sfd;
    res.packet_len = p_state->packet_size;
    res.size_of_addr = size_of_addr;

    Request **request_container = &p_state->current_request;

    uint32_t packet_index = process_pdu_header(packet, res, request_container, p_state->request_list, p_state);
    p_state->current_request = (*request_container);

    if (packet_index == 0)
        return -1;

    parse_packet_server(packet, packet_index, res, (*request_container), p_state);

    memset(packet, 0, res.packet_len);
    return 0;

}

static int on_recv_client(int sfd, char *packet, uint32_t *buff_size, void *addr, size_t size_of_addr, void *other) {
    

    Client *client = (Client *) other;

    Response res;
    res.addr = addr;
    res.sfd = sfd;
    res.packet_len = client->packet_len;
    res.size_of_addr = size_of_addr;

    Request **request_container = &client->current_request;

    uint32_t packet_index = process_pdu_header(packet, res, request_container, client->request_list, client->p_state);
    if (packet_index == -1) {
        ssp_printf("error parsing header\n");
        return -1;
    }
     
    res.msg = (*request_container)->buff;
    parse_packet_client(packet, packet_index, res, (*request_container), client);

    memset(packet, 0, res.packet_len);
    return 0;
    
}


static struct user_request_check_params {
    Response res;
    Client *client;
};

static void user_request_check(void *request, void *args) {
    Request * req = (Request *) request;
    struct user_request_check_params* params = (struct user_request_check_params *) args;

    params->res.msg = req->buff;
    user_request_handler(params->res, req, params->client);
}

static int on_send_client(int sfd, struct sockaddr_in addr, void *other) {


    #ifdef POSIX_PORT
    struct sockaddr_in* client_addr = (struct sockaddr_in*) &addr;
    #endif

    Response res;    
    Client *client = (Client *) other;

    res.sfd = sfd;
    res.packet_len = client->packet_len;
    res.addr = client_addr;
    
    struct user_request_check_params params = {
        res,
        client
    };

    client->request_list->print(client->request_list, user_request_check, &params);
    
    return 0;
}


static int timeout_remove_request(void *request, void *args) {
    Request *req = (Request *) request;
    Request *req2 = (Request *) args;
    if (req->dest_cfdp_id == req2->dest_cfdp_id && req->transaction_sequence_number == req2->transaction_sequence_number)
        return 1;
    return 0;
}

static void timeout_check(void *request, void *args) {
    Request *req = (Request *) request;
    List *req_list = (List *) args;

    on_server_time_out(req->res, req); 

    if (!req->is_active) {
        Request *remove_this = req_list->remove(req_list, 0, timeout_remove_request, req);
        ssp_cleanup_req(remove_this);
        
    }

}
//this function is a callback when using  my posix ports
static int on_time_out_posix(void *other) {

    Protocol_state *p_state = (Protocol_state*) other;
    if(p_state->current_request == NULL)
        return 0;

    p_state->request_list->print(p_state->request_list, timeout_check, p_state->request_list);
    
    return 0;
}

static int check_exit_server(void *params) {
    Protocol_state *p_state = (Protocol_state*) params;
    if (p_state->close)
        return 1;
    return 0;
}

static int check_exit_client(void *params) {
    Client *client = (Client*) params;
    if (client->close)
        return 1;
    return 0;
}

static void on_exit_client (void *params) {
    Client *client = (Client*) params;
    ssp_cleanup_client(client);
}

static void on_exit_server (void *params) {
    Protocol_state *p_state = (Protocol_state*) params;
    ssp_cleanup_p_state(p_state);
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
        udpSelectServer(p_state->server_port, PACKET_LEN, on_recv_server, on_time_out_posix, on_stdin, check_exit_server, on_exit_server, p_state);
    #endif

    return NULL;
}

    
void *ssp_connectionless_client_task(void* params){

    Client *client = (Client *) params;

    char host_name[INET_ADDRSTRLEN];
    char port[10];
    //convert int to char *
    snprintf(port, 10, "%d", client->unitdata_port);

    //convert uint id to char *
    inet_ntop(AF_INET, &client->unitdata_id, host_name, INET_ADDRSTRLEN);
    
    #ifdef POSIX_PORT
        udpClient(host_name, port, PACKET_LEN, client, client, client, client, on_send_client, on_recv_client, check_exit_client, on_exit_client);
    #endif
    return NULL;
}



/*------------------------------------------------------------------------------
    
    free functions

------------------------------------------------------------------------------*/




void ssp_cleanup_p_state(Protocol_state *p_state) {
    p_state->request_list->free(p_state->request_list, ssp_cleanup_req);
    free_mib(p_state->mib);
    ssp_free(p_state->server_port);
    ssp_free(p_state);

}


void ssp_cleanup_client(Client *client) {
    client->request_list->free(client->request_list, ssp_cleanup_req);
    ssp_cleanup_pdu_header(client->pdu_header);
    ssp_free(client);
}
