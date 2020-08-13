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
    sciInit();
    i2cInit();
    canInit();
    spiInit();
    gioInit();
    hetInit();

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
    dataconfig1_t.WDEL    = TRUE;
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0xFE;

    uint16_t data;
    int i;
    for(i = 0; i<500; i++){
        data = 0;
        spiReceiveData(regset, &dataconfig1_t, 1, &data);
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
