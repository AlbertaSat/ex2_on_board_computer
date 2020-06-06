#include "unity.h"

#include "adc_handler.h"
#include "mock_spi.h"

ADC_Handler handle;

void setUp(void)
{
    spiInit_Expect();
    adc_init(&handle, spiREG3);
}

void tearDown(void)
{
}

void test_adc_handler_SetControlReg(void)
{
    spiTransmitData_ExpectAnyArgsAndReturn(0);

    adc_set_control_reg(&handle, 0x0FFF);

    TEST_ASSERT_EQUAL_INT(4096, handle.adc_res);
    TEST_ASSERT_EQUAL_HEX16(0x0FFF, handle.control_reg_val);  
}

void test_adc_handler_getVal(void) 
{
    uint16_t val = 0;

    spiReceiveData_ExpectAnyArgsAndReturn(0x7FFF);

    val = adc_get_val(&handle);

    TEST_ASSERT_EQUAL_HEX16(0x7FFF, val);
}
