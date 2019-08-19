//the management information base (MIB)

//default faul handle


#ifndef MIB_H
#define MIB_H

#include <stdint.h>
#include "packet.h"
#include "types.h"

MIB *init_mib(void);
void free_mib(MIB *mib);
int add_new_cfdp_entity(MIB *mib, uint32_t cfdp_id, uint32_t UT_address, uint16_t port);
Pdu_header *get_header_from_mib(MIB *mib, uint32_t dest_id, uint32_t source_id);
void ssp_cleanup_pdu_header(Pdu_header *pdu_header);
Remote_entity *get_remote_entity(MIB *mib, uint32_t dest_id);
#endif