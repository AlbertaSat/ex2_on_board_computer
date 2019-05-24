
/*------------------------------------------------------------------------------
CMPT-361-AS50(1) - 2017 Fall - Introduction to Networks
Assignment #2
Evan Giese 1689223

This is my utils.c file, it contains useful function and abstract data types
to use for general functionality. 
------------------------------------------------------------------------------*/
#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <libgen.h>
#include "port.h"



#define MAX_LEN 255
#define ID_LEN 10

//see header file
int checkAlloc(void *mem, int notOkToFail)
{

    if (mem == NULL && notOkToFail)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    else if(mem == NULL && !notOkToFail) {
        return 0;
    }
    return 1;
}


//see header file
Config *configuration(int argc, char **argv)
{
    int ch;
    Config *conf = calloc(sizeof(Config), 1);
    checkAlloc(conf, 1);

    conf->timer = 15;
    conf->verbose_level = 0;
    conf->client_cfdp_id = 0;
    conf->my_cfdp_id = 0;

    uint32_t tmp;
    while ((ch = getopt(argc, argv, "t: i: c: v: h")) != -1)
    {
        switch (ch)
        {
        case 't':
            tmp = strtol(optarg, NULL, 10);
            conf->timer = tmp;
            break;

        case 'i':
            tmp = strtol(optarg, NULL, 10);
            conf->my_cfdp_id = tmp;
            break;
        
        case 'v':
            tmp = strtol(optarg, NULL, 10);
            conf->verbose_level = (uint8_t) tmp;
            break;

        case 'c': 
            tmp = strtol(optarg, NULL, 10);
            conf->client_cfdp_id = tmp;
            break;

        case 'h':
            printf("\n-----------HELP MESSAGE------------\n");
            printf("\nusage: %s [options] \n\n", basename(argv[0]));
            printf("Options: %s%s%s%s\n",
                    "-t timeout\n",
                    "-i my cfdp id for server\n",
                    "-c client id\n",
                    "-v verbose level (1-3)"
                    "-h HelpMessage");

            printf("Default port number mis 1111\n");
            printf("\n---------------END----------------\n");
            break;
        default:
            printf("\ngot something not found using default\n");
            break;
        }
    }
    return conf;
}



/*------------------------------------------------------------------------------
    This function creates a new node to add into the linked list, returns the
    new node, or NULL if failed
------------------------------------------------------------------------------*/

NODE *createNode(void *element, uint32_t id)
{
    NODE *newNode = calloc(sizeof(NODE), 1);
    if(!checkAlloc(newNode, 0))
        return NULL;

    newNode->element = element;
    newNode->id = id;
    return newNode;
}




/*------------------------------------------------------------------------------
    This function creates a new node to add into the linked list, returns the
    new node. 
------------------------------------------------------------------------------*/


static void freeNode(NODE *node) {
    ssp_free(node);    
}

static void *pop(List *list) {

    if (list->count == 0)
        return;

    NODE *last_data_node = list->tail->prev;
    if (last_data_node == NULL)
        return NULL;

    NODE *prev = last_data_node->prev;
    prev->next = list->tail;
    list->tail->prev = prev;

    list->count--;
    void *element = last_data_node->element;
    freeNode(last_data_node);
    return element;
}


/*------------------------------------------------------------------------------
    This function creates a new node to add into the beginning of the
    linked list, returns the new node. 
------------------------------------------------------------------------------*/

static int insert(List *list, void *element, uint32_t id) {

    NODE *head = list->head;
    NODE *node = createNode(element, id);
    if (node == NULL) {
        return 0;
    }
    if (list->count == 0) {
        list->tail->prev = node;
    }

    node->next = head->next;
    node->prev = head;

    head->next = node;
    list->count++;
    return 1;
}

/*------------------------------------------------------------------------------
    This function adds a new element into the linked list. returns 1 if success
    0 if failed.
------------------------------------------------------------------------------*/

static int push(List *list, void *element, uint32_t id)
{

    NODE *newNode = createNode(element, id);
    if (newNode == NULL) {
        return 0;
    }
    NODE *tail = list->tail;

    newNode->next = tail;
    newNode->prev = tail->prev;

    tail->prev->next = newNode;
    tail->prev = newNode;

    if (list->count == 0) {
        list->head->next = newNode;
    }
    
    list->count++;
    return 1;
}

/*------------------------------------------------------------------------------
    This function will print out the list if given a callback that is designed
    to print out an element.
------------------------------------------------------------------------------*/

static void printList(List *list, void (*f)(void *element, void *args), void *args)
{
    NODE *cur = list->head->next;
    while (cur->next != NULL)
    {
        f(cur->element, args);
        cur = cur->next;
    }
}


/*------------------------------------------------------------------------------
    This function removes an element from the linked list, returns the element stored if success
    and NULL if item not found. it can use either an id, or callback to find the
    element (callback can be the find function)
------------------------------------------------------------------------------*/

static void *removeElement(List *list, uint32_t id, int (*f)(void *element, void *args), void *args)
{
    NODE *cur = list->head->next;
    int found_with_func = 0;
    int found_with_id = 0;
    while (cur->next != NULL)
    {
        if (f != NULL)
            found_with_func = f(cur->element, args);

        if (id == cur->id)
            found_with_id = 1;

        if (found_with_func || found_with_id)
        {
            NODE *previous = cur->prev;
            NODE *next = cur->next;

            previous->next = next;
            next->prev = previous;
            void *element = cur->element;
            
            freeNode(cur);
            list->count--;
            return element;
        }
        cur = cur->next;
    }
    return NULL;
}



/*------------------------------------------------------------------------------
    frees the linked list. Takes a free function that is a function pointer to 
    a function that frees and elemnent.  Returns nothing,
------------------------------------------------------------------------------*/

static void freeList(List *list, void (*f)(void *element))
{
    NODE *cur = list->head->next;
    while (cur->next != NULL )
    {
        NODE *n = cur;
        cur = cur->next;
        f(n->element);
        freeNode(n);
    }
    free(list->head);
    free(list->tail);
    free(list);
}

/*------------------------------------------------------------------------------
    This function finds an element, returns and element on success and NULL on
    failure. The return value should be cast to the element type. can search with
    either a callback, or id
------------------------------------------------------------------------------*/

static void *findElement(List *list, uint32_t id, int (*f)(void *element, void *args), void *args)
{

    NODE *cur = list->head->next;
    int found_with_func = 0;
    int found_with_id = 0;
    while (cur->next != NULL)
    {
        if (f != NULL)
            found_with_func = f(cur->element, args);

        if(id != -1 && cur->id == id)
            found_with_id = 1;

        if (found_with_func || found_with_id){
            return cur->element;

        }
            
        cur = cur->next;
    }
    return NULL;
}

static int insertAt(List *list, void *element, uint32_t id, int (*f)(void *element, void *args), void *args) {
  
    NODE *cur = list->head->next;
    int found_with_func = 0;
    int found_with_id = 0;
    while (cur->next != NULL)
    {
        if (f != NULL)
            found_with_func = f(cur->element, args);

        if(id != -1 && cur->id == id)
            found_with_id = 1;

        if (found_with_func || found_with_id) {
            NODE *new = createNode(element, id);
            new->next = cur;
            new->prev = cur->prev;
            new->prev->next = new;
            cur->prev = new;
            list->count++;
        }
            
        cur = cur->next;
    }
    return -1;
}

 
static NODE * findNode(List *list, uint32_t id, int (*f)(void *element, void *args), void *args) {

    
    NODE *cur = list->head->next;
    int found_with_func = 0;
    int found_with_id = 0;
    while (cur->next != NULL)
    {
        if (f != NULL)
            found_with_func = f(cur->element, args);

        if(id != -1 && cur->id == id)
            found_with_id = 1;

        if (found_with_func || found_with_id){
            return cur;

        }
            
        cur = cur->next;
    }
    return NULL;

}
//see header file return NULL if fails

List *linked_list()
{
    List *newList = calloc(sizeof(List), 1);
    if(!checkAlloc(newList, 0))
        return NULL;

    newList->head = createNode(NULL, 0);
    if (newList->head == NULL) 
        return NULL;

    newList->tail = createNode(NULL, 0);
    if (newList->tail == NULL)
        return NULL;
    
    NODE *tail = newList->tail;
    NODE *head = newList->head;

    tail->prev = head;
    tail->next = NULL;
    head->next = tail;
    head->prev = NULL;

    newList->push = push;
    newList->remove = removeElement;
    newList->print = printList;
    newList->free = freeList;
    newList->insert = insert;
    newList->pop = pop;
    newList->find = findElement;
    newList->insertAt = insertAt;
    newList->findNode = findNode;
    return newList;
}


//TODO write an array
/*
int add_element_array_list(List *list, void *element) {
}

List *array_list() {

    List *new_list = calloc(sizeof(List), 1);
    if (checkAlloc(new_list, 0));
        return NULL;

}

*/

