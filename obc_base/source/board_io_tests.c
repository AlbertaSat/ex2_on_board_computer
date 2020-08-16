/*
 * board_io_tests.c
 *
 *  Created on Aug 13, 2020
 *
 *      Author: joshdellaz
 */


#include "HL_het.h"
#include "HL_gio.h"
#include "HL_i2c.h"
#include "HL_sci.h"
#include "HL_can.h"
#include "HL_spi.h"

void InitIO(void){
    i2cInit();
    sciInit();
    canInit();
    spiInit();
    gioInit();

    //The following sets the proper direction for all GPIO
    gioSetDirection(hetPORT2, 0xFFFFFFEA);
    gioSetDirection(hetPORT1, 0x9CFF6BEF);
    gioSetDirection(gioPORTA, 0xFFFFFF6F);
    gioSetDirection(gioPORTB, 0xFFFFFFFF);

    gioSetBit(hetPORT2, 12, 1);//solar panel power - enable
    gioSetBit(hetPORT1, 20, 1);//IRIS nCONFIG - disable


    //hetInit();

}


/*  @param[in] bit number 0-13 that specifies the output GPIO to be written to.

*   @param[in] value binary value to write to GPIO
*
*   Writes a value to the specified GPIO pin */

uint8_t GPIOSetBit(uint8_t bit, uint8_t value){
    switch(bit){
        case 0:
            gioSetBit(hetPORT2, 23, value);
            break;
        case 1:
            while(1);//cannot write to input
            break;
        case 2:
            gioSetBit(hetPORT2, 21, value);
            break;
        case 3:
            gioSetBit(hetPORT1, 9, value);
            break;
        case 4:
            gioSetBit(hetPORT1, 3, value);
            break;
        case 5:
            gioSetBit(hetPORT1, 7, value);
            break;
        case 6:
            gioSetBit(hetPORT2, 22, value);
            break;
        case 7:
            gioSetBit(hetPORT1, 1, value);
            break;
        case 8:
            while(1);//cannot write to input
            break;
        case 9:
            gioSetBit(hetPORT1, 20, value);
            break;
        case 10:
            while(1);//cannot write to input
            break;
        case 11:
            while(1);//cannot write to input
            break;
        case 12:
            gioSetBit(gioPORTB, 1, value);
            break;
        case 13:
            gioSetBit(gioPORTB, 0, value);
            break;
        default:
            return -1;
    }
    return 0;
}

/*  @param[in] bit number 0-13 that specifies the GPIO to be read.

*   @param[in] value binary value read from GPIO
*
*   Writes a value to the specified GPIO pin */
int GPIOGetBit(uint8_t bit){

    //GPIO0 - 2_23
    //GPIO1 - 1_25 - input
    //GPIO2 - 2_21
    //GPIO3 - 1_9
    //GPIO4 - 1_3
    //GPIO5 - 1_7
    //GPIO6 - 2_22
    //GPIO7 - 1_1
    //GPIO8 - 1_24 - input
    //GPIO9 - 1_20
    //GPIO10 - A7 - input
    //GPIO11 - 1_15 - input
    //GPIO12 - B1
    //GPIO13 - B0
    switch(bit){
        case 0:
            return gioGetBit(hetPORT2, 23);
        case 1:
            return gioGetBit(hetPORT1, 25);
        case 2:
            return gioGetBit(hetPORT2, 21);
        case 3:
            return gioGetBit(hetPORT1, 9);
        case 4:
            return gioGetBit(hetPORT1, 3);
        case 5:
            return gioGetBit(hetPORT1, 7);
        case 6:
            return gioGetBit(hetPORT2, 22);
        case 7:
            return gioGetBit(hetPORT1, 1);
        case 8:
            return gioGetBit(hetPORT1, 24);
        case 9:
            return gioGetBit(hetPORT1, 20);
        case 10:
            return gioGetBit(gioPORTA, 7);
        case 11:
            return gioGetBit(hetPORT1, 15);
        case 12:
            return gioGetBit(gioPORTB, 1);
        case 13:
            return gioGetBit(gioPORTB, 0);
        default:
            return -1;
    }
}


void GPIOTxTest(void){
    //note: this loops forever
    int i;
    while(1){
        for(i=0;i<=13;i++){
            if((i == 1) || (i == 8) || (i == 10) || (i == 11)){
                //do nothing
            }
            else{
                GPIOSetBit(i, 1);
            }
        }
        for(i=0;i<=13;i++){
            if((i == 1) || (i == 8) || (i == 10) || (i == 11)){
                //do nothing
            }
            else{
                GPIOSetBit(i, 0);
            }
        }
    }
}




void UARTTxTest(sciBASE_t * regset){

    int i;
    for(i = 0; i<1000; i++){
        sciSendByte(regset, 0b00110101);
    }

}

void UARTRxTest(sciBASE_t * regset){

    uint8_t data = 0;
    int i = 0;
    int pings = 0;

    while(i<1000){
        if(sciIsRxReady(regset) != 0){
            data = 0;
            data = sciReceiveByte(regset);
            if(data == 0b00110101){
                pings++;
            }
            i++;
        }
    }

}

void SPIMasterTxTest(spiBASE_t * regset){

    spiDAT1_t dataconfig1_t;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL    = FALSE;//not sure
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0x00;//no CS lines

    uint16_t data = 0b0011010100110101;
    int i;
    for(i = 0; i<500; i++){
        spiTransmitData(regset, &dataconfig1_t, 1, &data);
    }

}

void SPIMasterRxTest(spiBASE_t * regset){

    spiDAT1_t dataconfig1_t;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL    = FALSE;
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0x00;

    uint16_t data;
    int i = 0;
    while(i<500){
        data = 0;
        if(GPIOGetBit(8) == 0){//change to interrupt
            spiReceiveData(regset, &dataconfig1_t, 1, &data);
            if(data == 0b0011010100110101){
                i++;
            }
        }
    }

}

void SPISlaveTxTest(spiBASE_t * regset){

    //RUN THIS ON A DEV BOARD TO SEND DATA TO ATHENA
    //MAKE SURE TO SET THE BOARD TO BE A SLAVE

    spiDAT1_t dataconfig1_t;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL    = FALSE;//not sure
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0x00;//no CS lines

    uint16_t data = 0b0011010100110101;
    int i;
    for(i = 0; i<500; i++){
        spiTransmitData(regset, &dataconfig1_t, 1, &data);
    }

}

void SPISlaveRxTest(spiBASE_t * regset, uint8 spinumber){

    //Make this an interrupt thing when the CS line is enabled

    spiDAT1_t dataconfig1_t;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL    = FALSE;//not sure
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0x00;//no CS lines

    uint16_t data;
    int i = 0;
    while(i < 500){
        data = 0;
        spiReceiveData(regset, &dataconfig1_t, 1, &data);
        if(data == 0b0011010100110101){
            i++;
        }

    }

}

void I2CMasterTxTest(i2cBASE_t * regset, uint8_t addr){

    i2cSetSlaveAdd(regset, addr);

    i2cSetDirection(regset, I2C_TRANSMITTER);
    i2cSetCount(regset, 1000);
    i2cSetMode(regset, I2C_MASTER);
    i2cSetStop(regset);
    i2cSetStart(regset);
    int i;
    for(i = 0; i<1000; i++){
        i2cSendByte(regset, 0b00110101);
    }


    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(regset) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(regset) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(regset);

}

void I2CMasterRxTest(i2cBASE_t * regset, uint8_t addr){

    uint8_t data = 0;

    i2cSetSlaveAdd(regset, addr);
    /* Set direction to receiver */
    i2cSetDirection(regset, I2C_RECEIVER);
    i2cSetCount(regset, 1000);
    /* Set mode as Master */
    i2cSetMode(regset, I2C_MASTER);
    i2cSetStop(regset);
    /* Transmit Start Condition */
    i2cSetStart(regset);

    int i = 0;
    int pings = 0;
    while(i<1000){
        if(i2cIsRxReady(regset) != 0){
            data = 0;
            data = i2cReceiveByte(regset);
            if(data == 0b00110101){
                pings++;
            }
            i++;
        }
    }

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(regset) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(regset) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(regset);

}

void I2CSlaveRxTest(i2cBASE_t * regset, uint8_t addr){

    i2cSetMode(regset, I2C_SLAVE);
    i2cSetOwnAdd(regset, addr);
    i2cSetDirection(regset, I2C_RECEIVER);
    i2cSetCount(regset, 1000);

    uint8_t data = 0;
    int i = 0;

    while(i<1000){
        if(i2cIsRxReady(regset) != 0){
            data = i2cReceiveByte(regset);
            i++;
        }
    }

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(regset) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(regset) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(regset);

}

void I2CSlaveTxTest(i2cBASE_t * regset, uint8_t addr){

    i2cSetMode(regset, I2C_SLAVE);
    i2cSetOwnAdd(regset, addr);
    i2cSetDirection(regset, I2C_TRANSMITTER);
    i2cSetCount(regset, 1000);

    int i;
    for(i = 0; i<1000; i++){
        i2cSendByte(regset, 0b00110101);
    }

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(regset) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(regset) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(regset);

}

void CANTxTest(canBASE_t * regset, uint8_t msgbox){

    int i;

    uint8_t data[8] = {53, 53, 53, 53, 53, 53, 53, 53};

    for(i = 0; i<125; i++){
        while(canIsTxMessagePending(regset, msgbox));
        canTransmit(regset, msgbox, data);
    }

}

void CANRxTest(canBASE_t * regset, uint8_t msgbox){

    uint8_t data[8] = {0};
    int i = 0;

    while(i<125){
        while(!canIsRxMessageArrived(regset, msgbox));
        canGetData(regset, msgbox, data);
        i++;
    }

}
