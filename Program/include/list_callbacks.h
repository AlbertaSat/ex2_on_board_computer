

#ifndef LIST_CALLBACKS_H
#define LIST_CALLBACKS_H


    int client_cleanup_list(void *element);
    int client_list_remove(void *element, void *args);
    void client_list_print(void *element, void *args);
    int client_list_find(void *element, void *args);
    void client_list_free(void *element);

#endif


