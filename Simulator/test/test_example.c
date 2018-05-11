#include "unity.h"
#include "example.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_example(void)
{
    TEST_ASSERT_EQUAL_INT(-2, someFunc());
}
