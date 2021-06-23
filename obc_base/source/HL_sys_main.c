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

    uint8_t pa, mode;
    int stx_return;
    // 1a
    stx_return = STX_getControl(&pa, &mode);

        // 1b
//    uint8_t scrambler, filter, mod, rate;
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    rate = 1;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1c
//    mod = 0;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1d
//    filter = 1;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1e
//    scrambler = 1;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1f
//    uint8_t power = 24;
//    stx_return = STX_setPaPower(power);
//    stx_return = STX_getPaPower(&power);
//    // 1g
//    float frequency = 2250.5;
//    stx_return = STX_setFrequency(frequency);
//    stx_return = STX_getFrequency(&frequency);
//    // 1h
//    frequency = 22505.0;
//    stx_return = STX_setFrequency(frequency);
//    stx_return = STX_getFrequency(&frequency);
//    // 1i
//    // stx_return = STX_softResetFPGA();
//
//    stx_return = STX_getControl(&pa, &mode);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    stx_return = STX_getPaPower(&power);
//    stx_return = STX_getFrequency(&frequency);
    // 1j
    // Easiest way is to remove stx_return = STX_softResetFPGA();
    // reflash the code and put a breakpoint, do 1j
    // and continue the get functions.
    // 1k
    // Reflash the code (1i lines are redundant.)
//    rate = 0;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1l
//    mod = 1;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1m
//    filter = 0;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1n
//    scrambler = 0;
//    stx_return = STX_setEncoder(scrambler, filter, mod, rate);
//    stx_return = STX_getEncoder(&scrambler, &filter, &mod, &rate);
//    // 1o
//    power = 30;
//    stx_return = STX_setPaPower(power);
//    stx_return = STX_getPaPower(&power);
//    // 1p
//    frequency = 2200;
//    stx_return = STX_setFrequency(frequency);
//    stx_return = STX_getFrequency(&frequency);
//
//    // 2a
//    uint8_t pwrgd, txl;
//    stx_return = STX_getStatus(&pwrgd, &txl);
//    // 2b
//    int transmit;
//    stx_return = STX_getTR(&transmit);
//    // 2c
//    uint8_t quantity[3] = {0, 1, 2};
//    uint16_t ptr[3];
//    int i;
//    for (i=0; i<3; i++){
//        stx_return = STX_getBuffer(quantity[i], &ptr[i]);
//    }
//    // 2d - 2o
//    sBand_housekeeping hk;
//    stx_return = STX_getHK(&hk);

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
