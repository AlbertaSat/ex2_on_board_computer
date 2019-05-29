
#ifndef SSP_TASKS_H
#define SSP_TASKS_H



#include "protocol_handler.h"


void ssp_cleanup(Protocol_state *state);
void ssp_cleanup_client(Client *client);
void ssp_cleanup_req(void *request);
Protocol_state *ssp_connectionless_server(char *port);
Client *ssp_connectionless_client(uint32_t cfdp_id, Protocol_state *p_state);
void *ssp_connectionless_client_task(void *params);

















#endif