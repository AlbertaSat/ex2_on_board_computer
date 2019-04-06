/*------------------------------------------------------------------------------
CMPT-361-AS50(1) - 2017 Fall - Introduction to Networks
Assignment #1
Evan Giese 1689223

This file is the header file for utils.c
------------------------------------------------------------------------------*/
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <arpa/inet.h>

/*------------------------------------------------------------------------------
    Purpose: This struct if our configuration for this program, these elements
    are set with getopt
------------------------------------------------------------------------------*/
typedef struct config
{
    unsigned int timer;
    uint32_t client_cfdp_id;
    uint32_t my_cfdp_id;
    uint8_t verbose_level;
} Config;

/*------------------------------------------------------------------------------
    Purpose: This is a node for the link list bellow
------------------------------------------------------------------------------*/
typedef struct NODE
{
    uint32_t id;
    void *element;
    struct NODE *next;
    struct NODE *prev;
} NODE;

/*------------------------------------------------------------------------------
    Purpose: This is a LIST struct that contains functions useful for 
    doing linked list functionality. 
------------------------------------------------------------------------------*/
typedef struct List
{
    struct NODE *head;
    struct NODE *tail;
    int count;
    int (*push)(struct List *list, void *element,  uint32_t id);
    void *(*remove)(struct List *list, uint32_t id, int (*f)(void *element, void *args), void *args);
    void (*print)(struct List *list, void (*f)(void *element, void *args), void *args);
    void (*free)(struct List *list, void (*f)(void *element));
    void *(*pop) (struct List *list);
    int (*insert) (struct List *list, void *element, uint32_t id);
    int (*insertAt)(struct List *list, void *element, uint32_t id, int (*f)(void *element, void *args), void *args);
    //returns a void pointer that should be cast to the type
    void *(*find)(struct List *list, uint32_t id, int (*f)(void *element, void *args), void *args);
    struct NODE *(*findNode)(struct List *list, uint32_t id, int (*f)(void *element, void *args), void *args);

} List;

/*------------------------------------------------------------------------------
    Purpose:    This function initializes a linked list LIST *.
    Perameters: empty is just for the compiler errors, TODO use it for something
    Return:     returns a pointer to an initilized LIST * 
------------------------------------------------------------------------------*/
List *linked_list(void);

/*------------------------------------------------------------------------------
    Purpose:    This function checks to see if the memory is allocated,
                it exits if ok = 1 and keeps running in ok == 0 
    Perameters: void *mem: 
                int notOkToFail: 
    Return:     returns nothing
------------------------------------------------------------------------------*/
int checkAlloc(void *mem, int notOkToFail);

/*------------------------------------------------------------------------------
    Purpose:    This function is used to make a configuration struct from the
                arguments
    Perameters: int agrc: the number of arguments
                char **argv: the arguments
    Return:     CONFIG *
------------------------------------------------------------------------------*/
Config *configuration(int argc, char **argv);

//https://stackoverflow.com/questions/3022552/is-there-any-standard-htonl-like-function-for-64-bits-integers-in-c
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

NODE *createNode(void *element, uint32_t id);

#endif //UTILS_H
