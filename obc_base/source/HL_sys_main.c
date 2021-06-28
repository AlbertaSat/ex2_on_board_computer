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
#include "testing_CLI.h"
#include "board_io_tests.h"
#include "uTransceiver.h"

#include <stdlib.h>
#include "sTransmitter.h"
#include "spi.h"

#include <time.h>

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
//static unsigned char command;
/* USER CODE END */

uint8	emacAddress[6U] = 	{0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
uint32 	emacPhyAddress	=	1U;


 int main(void){

/* USER CODE BEGIN (3) */
    _enable_IRQ();
    InitIO();

    gioSetBit(hetPORT2, 22, 1); //Enables UHF transceiver
    gioSetBit(hetPORT2, 23, 1); //Enables S-band transmitter
    gioSetBit(hetPORT2, 21, 1); //Ensures the transmitter is not being reset

    int j_delay;
    for(j_delay = 0; j_delay < 0x800000; j_delay++);

    srand(time(0));
    uint8_t stx_return;
    uint8_t pa, mode;
    stx_return = STX_setControl(0,0);
    stx_return = STX_setControl(1,1);
    stx_return = STX_getControl(&pa, &mode);

//    //3a
//    uint16_t rand_byte, count;
//    int tr = 0;
//
//    rand_byte = rand() % 65536; // 16 bit number
//    SPISbandTx(&rand_byte);
//    stx_return = STX_getTR(&tr);
//    stx_return = STX_getBuffer(0, &count);
//
//    // 3b
//    int i = 0;
//    for(i; i < 1280; i++){
//        rand_byte = rand() % 65536; // 16 bit number
//        SPISbandTx(&rand_byte);
//    }
//
//    stx_return = STX_getTR(&tr);
//    stx_return = STX_getBuffer(0, &count);
//
//    // 3cP1
//    i = 0;
//    uint16_t stop_count, current_count;
//    stop_count = 500 + (rand() % 4500);
//    for(i; i < 5000; i++){
//        rand_byte = rand() % 65536;
//        SPISbandTx(&rand_byte);
//        if(i == stop_count){
//            gioSetBit(hetPORT1, 9, 1); // Disable the S-band CS line
//            break;
//        }
//    }
//    stx_return = STX_getTR(&tr);
//    stx_return = STX_getBuffer(0, &count);
//    current_count = count;
//    gioSetBit(hetPORT1, 9, 0); // Enable the S-band CS line
//
//    // 3cP2
//    i = 0;
//    uint16_t overrun;
//    for(i; i < (20480 - current_count); i++){
//        // Sending 16 bits at a time, so every second byte
//        if((i % 2) == 0){
//            rand_byte = rand() % 65536;
//            SPISbandTx(&rand_byte);
//        }
//    }
//    stx_return = STX_getTR(&tr);
//    stx_return = STX_getBuffer(0, &count);
//    stx_return = STX_getBuffer(2, &overrun);
//
//    // 3d
//    i = 0;
//    for(i; i < 50; i++){
//        rand_byte = rand() % 65536;
//        SPISbandTx(&rand_byte);
//    }
//    stx_return = STX_getTR(&tr);
//    stx_return = STX_getBuffer(0, &count);
//    stx_return = STX_getBuffer(2, &overrun);
//
//
//    // 4a
//    int j = 0;
//    int k = 0;
//    uint8_t command[2];
//
//
    uint8_t scrambler, filter, mod, rate, power;
    float freq;
//
//    while(k<100){
//        for(j; j<5; j++){
//            command[0] = j;
//            command[1] = rand() % 256;
//            i2c_sendCommand(0x26, command, 2);
//        }
//
//        stx_return = STX_getControl(&pa, &mode);
//        stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//        stx_return = STX_getPaPower(&power);
//        stx_return = STX_getFrequency(&freq);
//
//        stx_return = STX_softResetFPGA();
//
//        stx_return = STX_getControl(&pa, &mode);
//        stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//        stx_return = STX_getPaPower(&power);
//        stx_return = STX_getFrequency(&freq);
//        k++;
//        j = 0;
//    }
//
//    // 4b
//    uint8_t control_command[2] = {0, 124};
//    i2c_sendCommand(0x26, control_command, 2);
//    stx_return = STX_getControl(&pa, &mode);
//
//    // 4c
//    uint8_t encoder_command[3] = {1, 1, 1}; // Effectively 257
//    i2c_sendCommand(0x26, encoder_command, 3);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);

    // 4d
    stx_return = STX_getControl(&pa, &mode);
    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
    stx_return = STX_getPaPower(&power);
    stx_return = STX_getFrequency(&freq);

//    // 4e
//    i2cSetBaudrate(i2cREG1, 500);
//    stx_return = STX_getControl(&pa, &mode);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    stx_return = STX_getPaPower(&power);
//    stx_return = STX_getFrequency(&freq);

    // 4f will have to be done within the i2c code I think

    stx_return = STX_setControl(0,0);

    CLIhandler();

    while(1);

//    if (xTaskCreate(vTaskFileSys,"TaskFileSys", 1024, NULL, 1, &xTask1Handle) != pdTRUE)
//    {
//        /* Task could not be created */
//        while(1);
//    }

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */

//void vApplicationGetIdleTaskMemory(
//        StaticTask_t **ppxIdleTaskTCBBuffer,
//        StackType_t **ppxIdleTaskStackBuffer,
//        uint32_t *pulIdleTaskStackSize
//)
//{
///* If the buffers to be provided to the Idle task are declared inside this
//function then they must be declared static otherwise they will be allocated on
//the stack and so not exists after this function exits. */
//    static StaticTask_t xIdleTaskTCB;
//    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
//    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
//    state will be stored. */
//    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
//    /* Pass out the array that will be used as the Idle task's stack. */
//    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
//    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
//    Note that, as the array is necessarily of type StackType_t,
//    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
//    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
//}

/* USER CODE END */
