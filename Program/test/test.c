
#include "test.h"

#include "utils.h"
#include <stdio.h>

int test_num = 0;

void ASSERT_EQUALS_INT(char* description, int val1, int val2) {
    
    test_num++;
    if (val1 == val2){
        printf("\033[0;32m");
        printf("%s", description);
        printf(" pass # %d\n", test_num);
    }
    else {
        printf("\033[0;31m");
        printf("%s", description);
        printf(" fail # %d\n", test_num);
    }
    printf("\033[0m"); 
}


void ASSERT_NOT_EQUALS_INT(char* description, int val1, int val2) {
    
    test_num++;
    if (val1 == val2){
        printf("\033[0;31m");
        printf("%s", description);
        printf(" fail # %d\n", test_num);
    }
    else {
        printf("\033[0;32m");
        printf("%s", description);
        printf(" pass # %d\n", test_num);
    }
    printf("\033[0m"); 
}


void ASSERT_EQUALS_STR(char* description, char *val1,  char* val2, size_t size) {
    
    test_num++;
    if (!memcmp(val1, val2, size)) {
        printf("\033[0;32m");
        printf("%s", description);
        printf(" pass # %d\n", test_num);

    } else {
        printf("\033[0;31m");
        printf("%s", description);
        printf(" fail # %d\n", test_num);

    }
    printf("\033[0m"); 
}

void ASSERT_NOT_EQUALS_STR(char* description, char *val1,  char* val2, size_t size) {
    
    test_num++;
    if (!memcmp(val1, val2, size)) {

        printf("\033[0;31m");
        printf("%s", description);
        printf(" fail # %d\n", test_num);

    } else {
        printf("\033[0;32m");
        printf("%s", description);
        printf(" pass # %d\n", test_num);
    }
    printf("\033[0m"); 
}
