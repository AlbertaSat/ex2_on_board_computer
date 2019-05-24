
#ifndef PORT_H
#define PORT_H

#define STACK_ALLOCATION 16384
#define POSIX_PORT 1
#define MAX_PATH 255
#include "protocol_handler.h"


void ssp_error( char *msg);
Protocol_state *ssp_connectionless_server(char *port);
void *ssp_connectionless_server_task(void *protocol_state);


Client *ssp_connectionless_client(uint32_t cfdp_id, Protocol_state *p_state);
void *ssp_connectionless_client_task(void *params);
void ssp_printf(char *stuff, ...);

Request *init_request(uint32_t buff_len);
void *ssp_alloc(uint32_t u_memb, size_t size);
void ssp_sendto(Response res);
void ssp_cleanup(Protocol_state *state);
void ssp_cleanup_client(Client *client);
void ssp_cleanup_req(Request *req);
void ssp_print_hex(char *stuff, uint32_t size);
int ssp_open(char *pathname, int flags);
int ssp_read(int fd, char* buff, size_t size);
int ssp_lseek(int fd, int offset, int whence);
void ssp_error(char *error);
int ssp_write(int fd, const void *buf, size_t count);
int ssp_close(int fd);
void ssp_free(void *pointer);

int ssp_rename(const char *old, const char *new);

void ssp_thread_cancel(void *thread_handle);

void ssp_cleanup_pdu_header(Pdu_header *pdu_header);
void reset_request(Request *req);

#endif