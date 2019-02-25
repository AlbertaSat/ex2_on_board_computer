

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

    list->add(list, st);
    list->add(list, st2);
    list->add(list, st3);

    
    int id = 1;
    struct myStruct *found = list->find(list, find, &id);
    printf("found %d\n", found->b);

    id = 2;
    found = list->find(list, find, &id);
    printf("found %d\n", found->b);

    id = 3;
    found = list->find(list, find, &id);
    printf("found %d\n", found->b);


    list->print(list, list_print, NULL);


    list->remove(list, list_remove, &id);  


    list->print(list, list_print, NULL);


    list->free(list, list_free);

    return 0;
}