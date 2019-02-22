
#ifndef PROTOCOL_H
#define PROTOCOL_H




typedef struct client {
    int sfd;

    char *msg;
    //this is type (struct sockaddr_in) in posix
    void *addr;
    int packet_len;

    char* host_name;
    char* client_port;

    void *client_handle;
    void *client_thread_attributes;


} Client;


typedef struct protocol_state {
    int packet_size;

    char* server_port;
    void *server_handle;
    void *server_thread_attributes;

} Protocol_state;



void packet_handler(void);



#endif