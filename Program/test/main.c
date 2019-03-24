

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

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

    struct myStruct *st = (struct myStruct *)element;
    printf("list print: %d\n", st->id);


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

    char **ptr,*ptr1,*ptr2,*ptr3;

    ptr = malloc(sizeof(char*) * 2);
    ptr1 = malloc(1);
    ptr2 = malloc(1);

    *ptr1 = 'h';
    *ptr2 = 'i';

    ptr[0] = ptr1;
    ptr[1] = ptr2;

    printf("%c%c\n", *ptr[0], *ptr[1]);


    /*
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