

#include "protocol_handler.h"
#include "stdlib.h"
#include "mib.h"
#include "test.h"

static char *build_mock_packet(Protocol_state *p_state, uint32_t id) {

    char *packet = calloc(sizeof(char*), 2000);
    Pdu_header *pdu_header = get_header_from_mib(p_state->mib, 1, id);
    build_pdu_header(packet, 1, 0, pdu_header);
    ssp_cleanup_pdu_header(pdu_header);
    return packet;
}

static int test_process_pdu_header() {


    Protocol_state *p_state = init_ftp(1);
    Request **req_container = &p_state->current_request; 
    
    //test 1
    char *packet = build_mock_packet(p_state, 2);
    process_pdu_header(packet, req_container, p_state->request_list, p_state);
    ASSERT_EQUALS_INT("request transaction number should equal", (*req_container)->transaction_sequence_number, 1);
    ASSERT_EQUALS_INT("souce id should equal", (*req_container)->dest_cfdp_id, 2);

    //test 2
    char *packet2 = build_mock_packet(p_state, 3);
    process_pdu_header(packet2, req_container, p_state->request_list, p_state);
    ASSERT_EQUALS_INT("request transaction number should equal", (*req_container)->transaction_sequence_number, 1);
    ASSERT_NOT_EQUALS_INT("souce id should not equal", (*req_container)->dest_cfdp_id, 2);
    ASSERT_EQUALS_INT("souce id should equal", (*req_container)->dest_cfdp_id, 3);

    free(packet);
    free(packet2);
    ssp_cleanup_p_state(p_state);
}


int protocol_handler_test() {
    int error = 0;
    error = test_process_pdu_header();
    return error;
}