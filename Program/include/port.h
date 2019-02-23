
#include "packet.h"

#ifndef PORT_H
#define PORT_H

#define STACK_ALLOCATION 16384
#define POSIX_PORT 1
#include "protocol_handler.h"


void ssp_error(char *msg);
Protocol_state *ssp_connectionless_server(char *port);
void *ssp_connectionless_server_task(void *protocol_state);


Client *ssp_connectionless_client(char *host_name, char*port, Protocol_state *p_state);
void *ssp_connectionless_client_task(void *params);
void ssp_printf(char*stuff, ...);
void ssp_sendto(Response res);
void ssp_cleanup(Protocol_state *state);
void ssp_cleanup_client(Client *client);
#endif