

#include "protocol_handler.h"
#include "stdlib.h"
#include "mib.h"

static char *build_mock_packet(Protocol_state *p_state, uint32_t id) {

    char *packet = calloc(sizeof(char*), 2000);
    Pdu_header *pdu_header = get_header_from_mib(p_state->mib, 1, id);
    build_pdu_header(packet, 1, 0, pdu_header);
    return packet;
}

static int test_process_pdu_header() {


    Protocol_state *p_state = init_ftp(1);
    Request **req_container = &p_state->current_request; 
    
    char *packet = build_mock_packet(p_state, 2);
    process_pdu_header(packet, req_container, p_state->request_list, p_state);
    


    free(packet);
    ssp_cleanup_p_state(p_state);
}


int protocol_handler_test() {
    int error = 0;
    error = test_process_pdu_header();
    return error;
}