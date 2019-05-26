#include "protocol_handler.h"
#include "port.h"
#include "requests.h"
#include "string.h"


static void list_print(void *element, void *args) {

    Request *req = (Request *) element;
    ssp_printf("%s\n", req->source_file_name);
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