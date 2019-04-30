
#ifndef PACKET_TEST_H
#define PACKET_TEST_H
#include <stdio.h>

void ASSERT_EQUALS_STR(char* description, char *val1,  char* val2, size_t size);
void ASSERT_NOT_EQUALS_INT(char* description, int val1, int val2);
void ASSERT_EQUALS_INT(char* description, int val1, int val2);
void ASSERT_NOT_EQUALS_STR(char* description, char *val1,  char* val2, size_t size) ;

#endif