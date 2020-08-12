/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "os_task.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_het.h"
#include "HL_gio.h"
#include "HL_i2c.h"
#include "HL_sci.h"
#include "HL_can.h"
#include "HL_spi.h"

/* Define Task Handles */
xTaskHandle xTask1Handle;

/* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        /* Taggle HET[1] with timer tick */
        gioSetBit(hetPORT1, 18, gioGetBit(hetPORT1, 18) ^ 1);
        vTaskDelay(100);
    }
}

void InitIO(void){
    sciInit();
    i2cInit();
    canInit();
    spiInit();
    gioInit();
    hetInit();

    //Experimental

}

void UARTTxTest(sciBASE_t * regset){

    int i;
    for(i = 0; i<1000; i++){
        sciSendByte(regset, 0b00110101);
    }

}

void UARTRxTest(sciBASE_t * regset){

    uint8_t data[1000] = {0};
    int i = 0;

    while(i<1000){
        if(sciIsRxReady(regset) != 0){
            data[i] = sciReceiveByte(regset);
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

    uint8_t val[1000] = {0};

    i2cSetSlaveAdd(regset, addr);
    /* Set direction to receiver */
    i2cSetDirection(regset, I2C_RECEIVER);
    i2cSetCount(regset, 1);
    /* Set mode as Master */
    i2cSetMode(regset, I2C_MASTER);
    i2cSetStop(regset);
    /* Transmit Start Condition */
    i2cSetStart(regset);
    *val = i2cReceiveByte(regset);

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

    uint8_t data[1000] = {0};
    int i = 0;

    while(i<1000){
        if(i2cIsRxReady(regset) != 0){
            data[i] = i2cReceiveByte(regset);
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



/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

uint8	emacAddress[6U] = 	{0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */
    InitIO();

    SPITxTest(spiREG5);



//    /* Set high end timer GIO port hetPort pin direction to all output */
//    gioInit();
//    gioSetDirection(gioPORTB, 0xFFFFFFFF);
//
//
//    /* Create Task 1 */
//    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
//    {
//        /* Task could not be created */
//        while(1);
//    }
//
//    /* Start Scheduler */
//    vTaskStartScheduler();
//
//    /* Run forever */
//    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
