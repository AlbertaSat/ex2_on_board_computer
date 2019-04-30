

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "filesystem_funcs.h"
#include "port.h"
#include "test.h"
#include "mib.h"
#include "packet_tests.h"

struct myStruct {
    int id;
    int b;
};


static int list_remove(void *element, void *args) {

    int id = *(int *) args;
    struct myStruct *st = (struct myStruct *)element;
    
    if (id == ((struct myStruct*) element)->id) {
        free(st);
        return 1;

    }
    return 0;
}

static void list_print(void *element, void *args) {

    Offset *offset = (Offset *) element;
    printf("missing offsets:start:%u end:%u\n", offset->start, offset->end);


}
static int find(void *element, void *args) {

    int id = *(int *) args;
    if (id == ((struct myStruct*) element)->id)
        return 1;
    return 0;
}


static void list_free(void *element) {
    free(element);
}


int main () {
    

    
    MIB *mib = init_mib();

    //setting host name for testing
    char *host_name = "127.0.0.1";
    uint32_t addr[sizeof(uint32_t)];
    inet_pton(AF_INET, host_name, addr);
    
    //adding new cfdp entities to management information base
    add_new_cfdp_entity(mib, 1315, *addr, 1111);
    add_new_cfdp_entity(mib, 2222, *addr, 1112);   

    //find server client in mib
    Remote_entity* server_entity = mib->remote_entities->find(mib->remote_entities, 1, NULL, NULL);
    if (server_entity == NULL) {
        printf("couldn't find entity\n");
    }


    Pdu_header *header = get_header_from_mib(mib, 2222, 1315);

    packet_tests(header);

    free_mib(mib);











































    /*test sequence of files
    File *file = create_file("test.txt");
    ssp_printf("file size: %u\n", file->total_size);
    
    uint32_t data_size = PACKET_LEN - 4;
    uint32_t data_start = 0;
    uint32_t data_end = data_size;

    for (int i=0; i < file->total_size; i+= data_size) {
        data_start = i;
        data_end = data_size + i;
        //ssp_printf("i:%d\n", data_end);
        receive_offset(file, 0, data_start, data_end); 
    }

    receive_offset(file, 0, 48516, 48833);
    /*

    //doesnt do this edge case because, would never request it?

    file->missing_offsets->print(file->missing_offsets, list_print, NULL);

    free_file(file);
    
    char *packet = "hello world";
    uint32_t checksum = calc_check_sum(packet, 10);
    printf("%d\n", checksum);

    File *file = create_file("test2.txt");
    
    checksum = check_sum_file(file, file->total_size, 20000);
    printf("%d\n", checksum);
    /*
    char **ptr,*ptr1,*ptr2,*ptr3;

    ptr = malloc(sizeof(char*) * 2);
    ptr1 = malloc(1);
    ptr2 = malloc(1);

    *ptr1 = 'h';
    *ptr2 = 'i';

    ptr[0] = ptr1;
    ptr[1] = ptr2;

    printf("%c%c\n", *ptr[0], *ptr[1]);


    
    List *list = linked_list();
    //static void *findElement(List *list, int (*f)(void *element, void *args), void *args)

    struct myStruct *st = malloc(sizeof(struct myStruct));
    struct myStruct *st2 = malloc(sizeof(struct myStruct));
    struct myStruct *st3 = malloc(sizeof(struct myStruct));

    st->id = 1;
    st->b = 2;

    st2->id = 2;
    st2->b = 3;

    st3->id = 3;
    st3->b = 4;

    list->push(list, st, 1);
    list->push(list, st2, 2);
    list->push(list, st3, 3);


    list->pop(list);
    list->print(list, list_print, NULL);  
    printf("\n\n");

    list->insert(list, st3, 3);
    list->print(list, list_print, NULL);  

    //list_free(list->pop(list));
    
    int id = 1;
    struct myStruct *found = list->find(list, 1, NULL, NULL);
    printf("found %d\n", found->id);

    id = 2;
    found = list->find(list, 2, find, &id);
    printf("found %d\n", found->id);

    found = list->find(list, 3, NULL, NULL);
    printf("found %d\n", found->id);

    free(list->remove(list, "3", NULL, NULL));
    free(list->remove(list, "2", NULL, NULL));  


    list->print(list, list_print, NULL);  
    list->free(list, list_free);
    */
    return 0;
}