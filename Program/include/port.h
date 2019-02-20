
#include "packet.h"

#ifndef PORT_H
#define PORT_H

#define POSIX_PORT 0
#include <sys/socket.h>
#include "protocol_handler.h"


void ss_error(char *msg);
void ss_connectionless_server(char *port);
void ss_sendto(Client client);


#endif