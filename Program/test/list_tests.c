


#include "list.h"
#include "list_tests.h"

#include "protocol_handler.h"
#include "requests.h"

#include "stdio.h"
#include "test.h"
#include "stdlib.h"
#include "string.h"

void print_list(void *element, void *args) {
    Request *r = (Request *) element;
    printf("source file name: %s\n", r->source_file_name);
}

void free_all(void *element){
    ssp_cleanup_req((Request *)element);
}
int list_tests() {

    List *list = linked_list();
    
    Request  *r = init_request(10000);
    Request *r2 = init_request(10000);

    memcpy(r->source_file_name, "mybestfriend", 12);
    memcpy(r2->source_file_name, "secondrequest", 12);

    list->push(list, r, 1);
    ASSERT_EQUALS_INT("should equal 1", 1, list->count);
    
    Request *r3 = (Request *) list->pop(list);
    ASSERT_EQUALS_STR("should equal mybestfriend", "mybestfriend", r3->source_file_name, 12);
    ASSERT_EQUALS_INT("should equal 0", 0, list->count);

    list->push(list, r2, 1);
    list->push(list, r, 2);
    ASSERT_EQUALS_INT("should equal 2", 2, list->count);

    //test removals
    Request *r4 = list->remove(list, 1, NULL, NULL);    
    ASSERT_EQUALS_INT("should equal 1", 1, list->count);
    ASSERT_EQUALS_STR("should equal secondrequest", "secondrequest", r4->source_file_name, 12);

    list->push(list, r4, 0);
    list->iterate(list, print_list, NULL);
    list->free(list, free_all);
    
    return 0;
}