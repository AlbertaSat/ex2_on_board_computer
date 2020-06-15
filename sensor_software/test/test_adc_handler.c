#include "unity.h"

#include "adc_handler.h"
#include "mock_spi.h"

ADC_Handler handle;

void setUp(void)
{
    adc_init(&handle, 4096);
}

void tearDown(void)
{
}

void test_adc_handler_getVal(void) 
{

    float temp = 0;
    uint16_t val = 0;

    adc_set_control_reg(&handle, 0,
                                 1,
                                 0,
                                 0,
                                 0);

    adc_get_raw(&handle, &val);

    temp = adc_get_temp(&handle, val, 2.5);

    //TEST_ASSERT_EQUAL_HEX16(0x7FFF, val);
    TEST_ASSERT_EQUAL_FLOAT(25.0, temp);
}
