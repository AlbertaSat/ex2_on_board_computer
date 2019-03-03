

#include "list_callbacks.h"
#include "protocol_handler.h"
#include "stdlib.h"
#include "port.h"

void client_list_free(void *element) {
    Client *client = (Client * ) element;

    ssp_cleanup_client(client);
}

void client_list_print(void *element, void *args) {

}


int client_list_find(void *element, void *args) {
/*
    int id = *(int *) args;
    if (id == ((struct myStruct*) element)->id)
        return 1;
    return 0;
    */
}
