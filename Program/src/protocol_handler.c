

#include "port.h"
#include "protocol_handler.h"



int list_remove(void *element, void *args) {

    /*
    int id = *(int *) args;
    struct myStruct *st = (struct myStruct *)element;
    
    if (id == ((struct myStruct*) element)->id) {
        free(st);
        return 1;

    }
    */

    return 0;
}

void list_print(void *element, void *args) {

}


int list_find(void *element, void *args) {
/*
    int id = *(int *) args;
    if (id == ((struct myStruct*) element)->id)
        return 1;
    return 0;
    */
}

void list_free(void *element) {
    Client * client = (Client*) element;
    ssp_cleanup_client(client);
}

void packet_handler_server(Response res, Protocol_state *p_state) {

    res.msg = "shutupa you\n";
    ssp_sendto(res);
}


void packet_handler_client(Response res, Protocol_state *p_state) {
    res.msg = "noa youa shuta up!\n";
    ssp_sendto(res);
}

//Omission of source and destination filenames shall indicate that only Meta
//data will be delivered

int put_request(char *dest_cfdp_id, 
            char *source_file_name,
            char *destination_file_name,
            uint8_t segmentation_control,
            uint8_t fault_handler_overides,
            uint8_t flow_lable,
            uint8_t transmission_mode,
            char* messages_to_user,
            char* filestore_requests            
            ) {


}

/*
(destination CFDP entity ID,
[source file name],
[destination file name],
[segmentation control],
[fault handler overrides],
[flow label],
[transmission mode],
[messages to user],
[filestore requests]) 
*/