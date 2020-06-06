#ifndef ADC_HANDLER_H
#define ADC_HANDLER_H

#include "stdint.h"
#include "spi.h"

typedef struct adc_handler_t ADC_Handler;

struct adc_handler_t{
    unsigned short      control_reg_val;   //Current value of the ADC control register
    unsigned int        adc_res;           //Maximum resolution of the ADC

    spiBASE_t          *spi_base_addr;
};


// Initialize ADC defaults and SPI
void adc_init(ADC_Handler *handl, spiBASE_t *spi);

//return the raw value from the adc
uint16_t adc_get_raw(ADC_Handler *handl);

unsigned int adc_get_temp();

void adc_set_control_reg(ADC_Handler *handl, unsigned short value);

//convert raw value to voltage
//unsigned int adc_();

//DEMUX PIN SETTING

#endif