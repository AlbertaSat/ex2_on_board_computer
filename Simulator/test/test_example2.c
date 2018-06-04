#include "unity.h"
#include "example2.h"

void setUp(void)
{
    printf("Setting Up Test2!\n");
}

void tearDown(void)
{
    printf("Tearing Down Test2!\n");
}

void test_example(void)
{
    TEST_ASSERT_EQUAL_INT(1, 1);
    TEST_ASSERT_EQUAL_INT(5, intAdd(2,3));
    TEST_ASSERT_EQUAL_INT(-1, intSub(2,3));
}

