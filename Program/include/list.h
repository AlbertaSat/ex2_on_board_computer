
#include <stdint.h>
#ifndef LIST_H
#define LIST_H
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
    void (*iterate)(struct List *list, void (*f)(void *element, void *args), void *args);
    void (*free)(struct List *list, void (*f)(void *element));
    void *(*pop) (struct List *list);
    int (*insert) (struct List *list, void *element, uint32_t id);
    int (*insertAt)(struct List *list, void *element, uint32_t id, int (*f)(void *element, void *args), void *args);
    //returns a void pointer that should be cast to the type
    void *(*find)(struct List *list, uint32_t id, int (*f)(void *element, void *args), void *args);
    struct NODE *(*findNode)(struct List *list, uint32_t id, int (*f)(void *element, void *args), void *args);
    void (*freeNode)(NODE *node); 

} List;

NODE *createNode(void *element, uint32_t id);
/*------------------------------------------------------------------------------
    Purpose:    This function initializes a linked list LIST *.
    Perameters: empty is just for the compiler errors, TODO use it for something
    Return:     returns a pointer to an initilized LIST * 
------------------------------------------------------------------------------*/
List *linked_list(void);

#endif