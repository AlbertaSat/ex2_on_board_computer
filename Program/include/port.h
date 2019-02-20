
#include "packet.h"

#ifndef PORT
#define PORT

#define POSIX_PORT 0
#include <sys/socket.h>

int on_time_out_posix(void *other);
int on_recv_posix(int sfd, char *msg, struct sockaddr_storage client, void *other);
void ss_connectionless_server(char *port);



#endif