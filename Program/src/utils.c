
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

//see header file
void checkAlloc(void *mem, int ok)
{
    if (mem == NULL && ok)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
}

//see header file
CONFIG *configuration(int argc, char **argv)
{
    int ch;
    CONFIG *conf = calloc(sizeof(CONFIG), 1);
    checkAlloc(conf, 1);
    conf->numOfDecks = 2;
    conf->startingMoney = 100;
    conf->timer = 15;
    conf->port = "4420";
    conf->minBet = 1;

    int tmp;
    while ((ch = getopt(argc, argv, "d: m: t: p: b: h")) != -1)
    {
        switch (ch)
        {
        case 'd':
            tmp = strtol(optarg, NULL, 10);
            if (0 < tmp  && tmp < 11)
                conf->numOfDecks = tmp;
            break;

        case 'm':
            tmp = strtol(optarg, NULL, 10);
            if (0 < tmp)
                conf->startingMoney = tmp;
            break;

        case 't':
            tmp = strtol(optarg, NULL, 10);
            if (10 < tmp && tmp < 45)
                conf->timer = tmp;
            break;

        case 'p':
            conf->port = optarg;
            break;

        case 'b':
            //tmp = strtol(optarg, NULL, 10);
            //if (0 < tmp && tmp < 4294967296)
             //   conf->minBet = tmp;

            break;

        case 'h':
            printf("\n-----------HELP MESSAGE------------\n");
            printf("\nusage: %s [options] \n\n", basename(argv[0]));
            printf("Options: %s%s%s%s%s\n",
                   "-d NumberOfDecks\n",
                   "-m StartingMoney\n",
                    "-t Timer(between 15 and 45 seconds)\n",
                    "-p PortNumber\n",
                    "-h HelpMessage");

            printf("default number of decks is 2, but can change \n%s%s%s%s%s",
                    "if to something between 1 and 10. Default \n",
                    "starting money is 100, and is good as long as it\n",
                    "between 0 and 4294967296 (uint32 max). \n",
                    "Default for timer is 15 seconds. Default port number\n",
                    "is 4420\n");
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
    new node.
------------------------------------------------------------------------------*/

static NODE *createNode(void *element)
{
    NODE *newNode = calloc(sizeof(NODE), 1);
    checkAlloc(newNode, 1);
    newNode->element = element;
    return newNode;
}


/*------------------------------------------------------------------------------
    This function adds a new element into the linked list. returns nothing.
------------------------------------------------------------------------------*/

static void addElement(LIST *list, void *element)
{

    NODE *newNode = createNode(element);
    NODE *tail = list->tail;

    newNode->next = tail;
    newNode->prev = tail->prev;

    tail->prev->next = newNode;
    tail->prev = newNode;
}

/*------------------------------------------------------------------------------
    This function will print out the list if given a callback that is designed
    to print out an element.
------------------------------------------------------------------------------*/

static void printList(LIST *list, void (*f)(void *element, void *args), void *args)
{
    NODE *cur = list->head->next;
    while (cur->next != NULL)
    {
        f(cur->element, args);
        cur = cur->next;
    }
}


/*------------------------------------------------------------------------------
    This function removes an element from the linked list, returns 1 if success
    and -1 if item not found.
------------------------------------------------------------------------------*/

static int removeElement(LIST *list, int (*f)(void *element, void *args), void *args)
{
    NODE *cur = list->head->next;
    while (cur->next != NULL)
    {
        int found = f(cur->element, args);
        if (found == 1)
        {
            NODE *previous = cur->prev;
            NODE *next = cur->next;
            previous->next = next;
            next->prev = previous;
            free(cur);
            return 1;
        }
        cur = cur->next;
    }
    return -1;
}

/*------------------------------------------------------------------------------
    frees the linked list. Takes a free function that is a function pointer to 
    a function that frees and elemnent.  Returns nothing,
------------------------------------------------------------------------------*/

static void freeList(LIST *list, void (*f)(void *element))
{
    NODE *cur = list->head->next;
    while (cur->next != NULL)
    {
        NODE *n = cur;
        cur = cur->next;
        f(n->element);
        free(n->element);
        free(n);
    }
    free(list->head);
    free(list->tail);
    free(list);
}

/*------------------------------------------------------------------------------
    This function finds an element, returns and element on success and NULL on
    failure. The return value should be cast to the element type.
------------------------------------------------------------------------------*/

static void *findElement(LIST *list, int (*f)(void *element, void *args), void *args)
{

    NODE *cur = list->head->next;
    while (cur->next != NULL)
    {
        int found = f(cur->element, args);
        if (found == 1)
        {
            return cur->element;
        }
        cur = cur->next;
    }
    return NULL;
}

//see header file

LIST *List()
{
    LIST *newList = calloc(sizeof(LIST), 1);
    checkAlloc(newList, 1);

    newList->head = createNode(NULL);
    newList->tail = createNode(NULL);

    NODE *tail = newList->tail;
    NODE *head = newList->head;

    tail->prev = head;
    head->next = tail;

    newList->add = addElement;
    newList->remove = removeElement;
    newList->print = printList;
    newList->free = freeList;
    newList->find = findElement;

    return newList;
}
