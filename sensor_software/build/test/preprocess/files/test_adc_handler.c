#include "build/temp/_test_adc_handler.c"
#include "build/test/mocks/mock_spi.h"
#include "src/adc_handler.h"
#include "/var/lib/gems/2.7.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"






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



    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((0x7FFF)), (UNITY_INT)(UNITY_INT16)((val)), (

   ((void *)0)

   ), (UNITY_UINT)(33), UNITY_DISPLAY_STYLE_HEX16);

}
