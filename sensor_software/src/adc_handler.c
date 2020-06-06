#include "adc_handler.h"


static spiDAT1_t sConfDat;


void adc_init(ADC_Handler *handl, spiBASE_t *spi) {
    //Initialize ADC defaults and SPI here
    handl->adc_res          = 4096;
    handl->control_reg_val  = 0;
    handl->spi_base_addr    = spi;
    //STORE SPI CS PIN ADDR

    //Initialize spi stuff
    spiInit();
    
    //These change for each ADC (different SPI CS line) MIGHT NEED TO MOVE/CHANGE THIS
    sConfDat.CS_HOLD = TRUE;
    sConfDat.WDEL    = TRUE;
    sConfDat.DFSEL   = SPI_FMT_0;
    sConfDat.CSNR    = 0xFF;
}

void adc_set_control_reg(ADC_Handler *handl, uint16_t value) {
    /*
     * create 8-bit buffer
     * split value into two 8-bit parts
     * set buffer values
     * 
     * 
     */
    // uint16_t buffer[2] = {0,0};

    // buffer[0] = (value & 0xFF00) >> 8;
    // buffer[1] = (value & 0x00FF);
    
    //SET CS PIN LOW
    //send buffer to SPI
    spiTransmitData(handl->spi_base_addr, &sConfDat, 1, &value);
    //SET CS PIN HIGH

    handl->control_reg_val = value;
}

uint16_t adc_get_raw(ADC_Handler *handl) {
    uint16_t value = 0;

    //SET CS PIN LOW 
    //SPI read into value
    spiReceiveData(handl->spi_base_addr, &sConfDat, 1, &value);
    //SET CS PIN HIGH

    return value;
}


