/*
 * @brief: ADC Handler - Returns voltage values from specified adc channel
 * 
*/
#ifndef ADC_HANDLER_H
#define ADC_HANDLER_H

#include "stdint.h"

typedef struct adc_handler_t ADC_Handler;

struct adc_handler_t{
    unsigned short      control_reg_val;   //Current value of the ADC control register
    const unsigned int  adc_res;           //Maximum resolution of the ADC
};

// Initialize ADC defaults and SPI
void adc_init(ADC_Handler *handl);

//return the raw value from the adc
unsigned int adc_get_val(ADC_Handler *handl);

void adc_set_control_reg(ADC_Handler *handl, unsigned short value);

//convert raw value to voltage
//unsigned int adc_();


#endif