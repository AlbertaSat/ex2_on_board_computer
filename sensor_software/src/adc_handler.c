#include "adc_handler.h"
#include "demux_handler.h"


void adc_init(ADC_Handler *handl, unsigned int res) {
    //Initialize ADC defaults and SPI here
    handl->adc_res          = res;
    handl->control_reg_val  = 0;
    //STORE SPI CS PIN ADDR

    //Initialize spi stuff
    // spiInit();
    
    // //These change for each ADC (different SPI CS line) MIGHT NEED TO MOVE/CHANGE THIS
    // sConfDat.CS_HOLD = TRUE;
    // sConfDat.WDEL    = TRUE;
    // sConfDat.DFSEL   = SPI_FMT_0;
    // sConfDat.CSNR    = 0xFF;
}

void adc_set_control_reg(ADC_Handler *handl, unsigned char repeat,
                                             unsigned char channel,
                                             unsigned char ext_ref,
                                             unsigned char tsense,
                                             unsigned char tsense_avg) {
    
    spi_write_ExpectAnyArgs(); //CMOCK

    uint16_t control_reg_value = 0;

    control_reg_value = AD7298_WRITE 
                      | (repeat     * AD7298_REPEAT) 
                      |  AD7298_CH(channel)
                      | (ext_ref    * AD7298_EXT_REF)
                      | (tsense     * AD7298_TSENSE)
                      | (tsense_avg * AD7298_TSENSE_AVG);

    //SET CS PIN LOW
    //send buffer to SPI
    spi_write(1, &control_reg_value);
    //SET CS PIN HIGH

    handl->control_reg_val = control_reg_value;
}

/*
 *  @brief Returns the raw value read from an ADC channel.
 * 
 *  @param handl - ADC Handler structure
 * 
 */
void adc_get_raw(ADC_Handler *handl, uint16_t *data) {  
    spi_read_ExpectAnyArgs(); //CMOCK

    uint16_t buffer = 0;

    //SET CS PIN LOW 
    //SPI read into value
    spi_read(1, &buffer);
    //SET CS PIN HIGH

    *data = buffer & 0x0FFF;
    //get data channel as well.
}

float adc_get_temp(ADC_Handler *handl, uint16_t value, float vref) {
    float temp_celsius = 0;

    if(value >= 0x800) {
        value  = handl->adc_res - value;
        value *= -1;
    }

    temp_celsius = vref * (((float)value / 10) + 109.3) - 273.15; //from datasheet

    return temp_celsius;
}

