#include "build/temp/_test_adc_handler.c"
#include "build/test/mocks/mock_spi.h"
#include "src/adc_handler.h"
#include "/var/lib/gems/2.7.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"




ADC_Handler handle;



void setUp(void)

{

    spiInit_CMockExpect(10);

    adc_init(&handle, ((spiBASE_t *)0xFFF7F800U));

}



void tearDown(void)

{

}



void test_adc_handler_SetControlReg(void)

{

    spiTransmitData_CMockExpectAnyArgsAndReturn(20, 0);



    adc_set_control_reg(&handle, 0x0FFF);



    UnityAssertEqualNumber((UNITY_INT)((4096)), (UNITY_INT)((handle.adc_res)), (

   ((void *)0)

   ), (UNITY_UINT)(24), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((0x0FFF)), (UNITY_INT)(UNITY_INT16)((handle.control_reg_val)), (

   ((void *)0)

   ), (UNITY_UINT)(25), UNITY_DISPLAY_STYLE_HEX16);

}



void test_adc_handler_getVal(void)

{

    uint16_t val = 0;



    spiReceiveData_CMockExpectAnyArgsAndReturn(32, 0x7FFF);



    val = adc_get_val(&handle);



    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((0x7FFF)), (UNITY_INT)(UNITY_INT16)((val)), (

   ((void *)0)

   ), (UNITY_UINT)(36), UNITY_DISPLAY_STYLE_HEX16);

}
