#ifndef ADC_HANDLER_H
#define ADC_HANDLER_H

#include "stdint.h"
#include "mock_spi.h"

// AD7298 Control Register Map
#define AD7298_WRITE        (1 << 15)
#define AD7298_REPEAT       (1 << 14)
#define AD7298_CH(x)        (1 << (13 - x))
#define AD7298_TSENSE       (1 << 5)
#define AD7298_EXT_REF      (1 << 2)
#define AD7298_TSENSE_AVG   (1 << 1)
#define AD7298_PPD          (1 << 0)

typedef struct adc_handler_t ADC_Handler;

struct adc_handler_t{
    uint16_t        control_reg_val;   //Current value of the ADC control register
    unsigned int    adc_res;           //Maximum resolution of the ADC
};

// Initialize ADC defaults and SPI
void adc_init(ADC_Handler *handl, unsigned int res);

//return the raw value from the adc
void adc_get_raw(ADC_Handler *handl, uint16_t *data);

float adc_get_temp(ADC_Handler *handl, uint16_t value, float vref);

void adc_set_control_reg(ADC_Handler *handl, unsigned char repeat,
                                             unsigned char channels,
                                             unsigned char ext_ref,
                                             unsigned char tsense,
                                             unsigned char tsense_avg);

//convert raw value to voltage
//unsigned int adc_();

//DEMUX PIN SETTING

#endif