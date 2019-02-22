
#include "packet.h"

#ifndef PORT_H
#define PORT_H

#define STACK_ALLOCATION 16384
#define POSIX_PORT 1
#include "protocol_handler.h"


void ssp_error(char *msg);
Protocol_state *ssp_connectionless_server(char *port);
void *ssp_connectionless_server_task(void *protocol_state);


Client *ssp_connectionless_client(char *host_name, char*port);
void *ssp_connectionless_client_task(void *params);

void ssp_sendto(Client client);
void ssp_cleanup(Protocol_state *state);
void ssp_cleanup_client(Client *client);
#endif