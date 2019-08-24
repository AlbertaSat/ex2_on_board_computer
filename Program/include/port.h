
#ifndef PORT_H
#define PORT_H

#define STACK_ALLOCATION 16384
#define POSIX_PORT 1
#define FREE_RTOS_PORT 1


#define MAX_PATH 255
#include "types.h"


void ssp_error( char *msg);

void ssp_printf(char *stuff, ...);

Request *init_request(uint32_t buff_len);
void *ssp_alloc(uint32_t u_memb, size_t size);
void ssp_sendto(Response res);

int ssp_recvfrom(int sfd, void *buff, size_t packet_len, int flags, void *server_addr, uint32_t server_addr_len);

void *ssp_thread_create(int stack_size, void * (thread_func)(void *params), void *params);

int ssp_open(char *pathname, int flags);
int ssp_read(int fd, char* buff, size_t size);
int ssp_lseek(int fd, int offset, int whence);
void ssp_error(char *error);
int ssp_write(int fd, const void *buf, size_t count);
int ssp_close(int fd);
void ssp_free(void *pointer);
void ssp_thread_join(void *thread_handle);


//network stuff
void ssp_fd_zero(void *socket_set);
void ssp_fd_set(int sfd, void *socket_set);
int ssp_fd_is_set(int sfd, void *socket_set);
void *ssp_init_socket_set();


int ssp_rename(const char *old, const char *new);
void reset_request(Request *req);

#endif