#include "protocol_handler.h"
#include "port.h"
#include "requests.h"
#include "string.h"
#include "test.h"


static void list_print_id(void *element, void *args) {
    Request *req = (Request *) element;
    printf("id: %d trans number: %d\n", req->dest_cfdp_id, req->transaction_sequence_number);
}

//for finding the struct in the list
struct request_search_params {
    uint32_t source_id;
    uint32_t transaction_sequence_number;
};

//for finding the struct in the list
static int find_request(void *element, void *args) {
    Request *req = (Request *) element;
    struct request_search_params *params = (struct request_search_params *) args;
    if (req->dest_cfdp_id = params->source_id && req->transaction_sequence_number == params->transaction_sequence_number)
        return 1;
    return 0;
}

static void list_print(void *element, void *args) {

    Request *req = (Request *) element;
    ssp_printf("%s\n", req->source_file_name);
}

int request_finding_test() {

    List *list = linked_list();

    Request *req = init_request(2000);
    req->dest_cfdp_id = 1;
    req->transaction_sequence_number = 1;
    list->push(list, req, req->dest_cfdp_id);

    Request *req2 = init_request(2000);
    req2->dest_cfdp_id = 3;
    req2->transaction_sequence_number = 1;
    list->push(list, req2, req2->dest_cfdp_id);

    Request *req3 = init_request(2000);
    req3->dest_cfdp_id = 2;
    req3->transaction_sequence_number = 2;
    list->push(list, req3, req3->dest_cfdp_id);


    struct request_search_params params = {
        req->dest_cfdp_id,
        req->transaction_sequence_number,
    };

    Request *found = list->find(list, 0, find_request, &params);
    params.source_id = 3;
    params.transaction_sequence_number = 1;
    Request *found2 = list->find(list, 0, find_request, &params);
    params.source_id = 2;
    params.transaction_sequence_number = 2;
    Request *found3 = list->find(list, 0, find_request, &params);

    if (found == NULL) {
        ssp_printf("CAN't FIND request IS NULL\n");
        list->free(list, ssp_cleanup_req);
        return 1;
    }
    
    ASSERT_EQUALS_INT("finding test, should equal", req->dest_cfdp_id, found->dest_cfdp_id);
    ASSERT_EQUALS_INT("finding test, should equal", req2->dest_cfdp_id, found2->dest_cfdp_id);
    ASSERT_EQUALS_INT("finding test, should equal", req3->dest_cfdp_id, found3->dest_cfdp_id);

    list->free(list, ssp_cleanup_req);
    return 0;
}


void request_test_list_storage() {
    Request *req = init_request(5000);
    List *list = linked_list();

    req->file = create_file("testfile.txt", 0);
    memcpy(req->source_file_name, "stuff", 6);
    list->insert(list, req, 1);

    Request *req2 = init_request(5000);
    req2->file = create_file("testfile.txt", 0);
    memcpy(req2->source_file_name, "stuff2", 7);
    list->insert(list, req2, 2);

    Request *req3 = init_request(5000);
    req3->file = create_file("testfile.txt", 0);
    memcpy(req3->source_file_name, "stuff3", 7);
    list->insert(list, req3, 3);

    ssp_cleanup_req(list->pop(list));
    list->print(list, list_print, NULL);  
    list->free(list, ssp_cleanup_req);

}


int request_tests() {

    int error = 0;
    error = request_finding_test(); 
    return 0;
}