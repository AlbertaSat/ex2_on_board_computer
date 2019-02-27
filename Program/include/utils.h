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
typedef struct CONFIG
{
    uint32_t numOfDecks;
    uint32_t startingMoney;
    int timer;
    char *port;
    uint32_t minBet;
} CONFIG;

/*------------------------------------------------------------------------------
    Purpose: This is a node for the link list bellow
------------------------------------------------------------------------------*/
typedef struct NODE
{
    char *id;
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
    int (*add)(struct List *list, void *element, char *id);
    void *(*remove)(struct List *list, char *id, int (*f)(void *element, void *args), void *args);
    void (*print)(struct List *list, void (*f)(void *element, void *args), void *args);
    void (*free)(struct List *list, void (*f)(void *element));

    //returns a void pointer that should be cast to the type
    void *(*find)(struct List *list, char *id, int (*f)(void *element, void *args), void *args);
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
CONFIG *configuration(int argc, char **argv);
#endif //UTILS_H
