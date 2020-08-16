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
#include "board_io_tests.h"
#include "ina226.h"
#include "deployablescontrol.h"
#include "demux_handler.h"
#include "rtcmk.h"
#include "tmp421.h"
#include "stdio.h"

///* Define Task Handles */
xTaskHandle xTask1Handle;
//
///* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        /* Taggle HET[1] with timer tick */
        gioSetBit(hetPORT1, 18, 1);
        vTaskDelay(100);
        gioSetBit(hetPORT1, 18, 0);
        vTaskDelay(100);
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
    bool status = 0;


    SPIMasterRxTest(spiREG5);


//    status = GPIOGetBit(1);
//    status = GPIOGetBit(1);
//
//    status = GPIOGetBit(8);
//    status = GPIOGetBit(8);
//
//    status = GPIOGetBit(10);
//    status = GPIOGetBit(10);
//
//    status = GPIOGetBit(11);
//    status = GPIOGetBit(11);
//
//
//    status = switchstatus(Port);
//    status = switchstatus(Port);
//
//    status = switchstatus(UHF_P);
//    status = switchstatus(UHF_P);
//
//    status = switchstatus(UHF_Z);
//    status = switchstatus(UHF_Z);
//
//    status = switchstatus(Payload);
//    status = switchstatus(Payload);
//
//    status = switchstatus(Starboard);
//    status = switchstatus(Starboard);
//
//    status = switchstatus(UHF_S);
//    status = switchstatus(UHF_S);
//
//    status = switchstatus(DFGM);
//    status = switchstatus(DFGM);
//
//    status = switchstatus(UHF_N);
//    status = switchstatus(UHF_N);




//    TMP421test(0x4C);


//    activateknife(PortKnife);
//    activateknife(UHFKnife_P);
//    activateknife(UHFKnife_Z);
//    activateknife(PayloadKnife);
//    activateknife(StarboardKnife);
//    activateknife(UHFKnife_S);
//    activateknife(DFGMKnife);
//    activateknife(UHFKnife_N);

//    int i;
//    while(1){

//        demux_disable()
//        demux_select_pin(OUT_Y0);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable()
//        demux_select_pin(OUT_Y1);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable();
//        demux_select_pin(OUT_Y2);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable();
//        demux_select_pin(OUT_Y3);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable();
//        demux_select_pin(OUT_Y4);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable();
//        demux_select_pin(OUT_Y5);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable();
//        demux_select_pin(OUT_Y6);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable();
//        demux_select_pin(OUT_Y7);
//        demux_enable();
//        for(i=0;i<100000;i++);
//        demux_disable();
//
//
//    }
//    i2cSetSlaveAdd(i2cREG2, 0x1D);
//    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
//    i2cSetCount(i2cREG2, 1);
//    i2cSetMode(i2cREG2, I2C_MASTER);
//    i2cSetStop(i2cREG2);
//    i2cSetStart(i2cREG2);
//    i2cSendByte(i2cREG2, 0xFF);
//    while(i2cIsBusBusy(i2cREG2) == true);
//    while(i2cIsStopDetected(i2cREG2) == 0);
//    i2cClearSCD(i2cREG2);

//    RTCMK_ResetTime(0x64);
//    uint8_t secs = 0;
//
//    RTCMK_ReadSeconds(0x64, &secs);


    //Calibrate device and set alert properly
//    uint16_t calibrationval = 0b0100111100111111;
//
//    if(INA226_RegisterSet(i2cREG2, 0x40, INA226_RegCalib, calibrationval)){
//        while(1);
//    }
//
//    int temp;
//    for (temp = 0; temp < 0x10000; temp++);//temporary fix... don't want delay down the road
//
//    uint16_t alertlimit = 0b0001111101000000;
//    uint16_t regval = 0;
//
//    if(INA226_RegisterSet(i2cREG2, 0x40, INA226_RegAlertLim, alertlimit)){
//        while(1);
//    }
////    for (temp = 0; temp < 0x10000; temp++);
////    if(INA226_RegisterGet(i2cREG2, 0x40, INA226_RegAlertLim, &regval)){
////        while(1);
////    }
////    if(regval != alertlimit){
////        fprintf(stderr, "alertlim register not set");
////    }
//
//    for (temp = 0; temp < 0x10000; temp++);
//
//    uint16_t alertsetting = 0b1000000000000000;
//
//    if(INA226_RegisterSet(i2cREG2, 0x40, INA226_RegMaskEn, alertsetting)){
//        while(1);
//    }
////    for (temp = 0; temp < 0x10000; temp++);
////    if(INA226_RegisterGet(i2cREG2, 0x40, INA226_RegMaskEn, &regval)){
////        while(1);
////    }
////    if(regval != alertsetting){
////        fprintf(stderr, "masken register not set");
////    }
//
//    for (temp = 0; temp < 0x10000; temp++);
//
//    uint16_t shuntv = 0;
//
//    if(INA226_RegisterGet(i2cREG2, 0x40, INA226_RegShuntV, &shuntv)){
//        while(1);
//    }
//
//
//    //obvs make this an interrupt thing in the future
//    while(1){
//        if (gioGetBit(hetPORT2, 0) == 0){
//            gioSetBit(hetPORT2, 12, 0);
//        }
//        else{
//            gioSetBit(hetPORT2, 12, 1);
//        }
//    }

//    int curr = 0;
//
//    if(INA226_ReadShunt(i2cREG2, 0x40, &curr) == -1){
//        while(1);
//    }



//    /* Set high end timer GIO port hetPort pin direction to all output */
//    gioInit();
//    gioSetDirection(gioPORTB, 0xFFFFFFFF);
//
//
//    /* Create Task 1 */
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
