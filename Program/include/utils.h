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

void ssp_print_hex(char *stuff, int size);

#endif //UTILS_H
