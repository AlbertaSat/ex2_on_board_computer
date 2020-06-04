#include "adc_handler.h"


void adc_init(ADC_Handler *handl) {
    //Initialize ADC defaults and SPI here
}

void adc_set_control_reg(ADC_Handler *handl, unsigned short value) {
    /*
     * create 8-bit buffer
     * split value into two 8-bit parts
     * set buffer values
     * 
     * 
     */
    unsigned char buffer[2] = {0,0};

    buffer[0] = (value & 0xFF00) >> 8;
    buffer[1] = (value & 0x00FF);
    
    //send buffer to SPI

    handl->control_reg_val = value;
}

unsigned int adc_get_val(ADC_Handler *handl) {
    unsigned int value = 0;
    
    //SPI read into value

    return value;
}


