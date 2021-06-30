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

    // Delay to alow the board after enabling
    int j_delay;
    for(j_delay = 0; j_delay < 0x800000; j_delay++);

    uint8_t stx_return;
    stx_return = STX_softResetFPGA();

//    //For I2C debugging Purposes
//    uint8_t test_scw[12] = {0};
//    while(1){
//        stx_return = STX_setControl(0,0);
//        //UHF_genericRead(0, test_scw);
//    }

    srand(time(0));

       // 5a
//    uint8_t papower;
//    stx_return = STX_getPaPower(&papower);
//       stx_return = STX_setControl(1, 1);
//       stx_return = STX_setControl(0, 1);
//
//       stx_return = STX_setControl(0, 0);
//       stx_return = STX_setPaPower(24);
//       stx_return = STX_setControl(1, 1);
//       stx_return = STX_getPaPower(&papower);
//
//       stx_return = STX_setControl(0, 0);
//       stx_return = STX_setPaPower(26);
//       stx_return = STX_setControl(1, 1);
//       stx_return = STX_getPaPower(&papower);
//
//       stx_return = STX_setControl(0, 0);
//       stx_return = STX_setPaPower(28);
//       stx_return = STX_setControl(1, 1);
//       stx_return = STX_getPaPower(&papower);
//
//       stx_return = STX_setControl(0, 0);
//       stx_return = STX_setPaPower(30);
//       stx_return = STX_setControl(1, 1);
//       stx_return = STX_getPaPower(&papower);
//
//       stx_return = STX_setControl(0, 0);
//       write_reg(0x03, 0x04);
//       stx_return = STX_setControl(1, 1);

//       // 5b
//       // 5c
//       stx_return = STX_setControl(1, 1);
//       // 5d
//       // 5e
//       // 10240 bytes in the buffer
       uint16_t count, underrun, overrun;
       int i = 0;
       uint16_t rand_byte;
//       for(i; i < 10240; i++){
//           rand_byte = rand() % 65536; // 16 bit number
//           SPISbandTx(&rand_byte);
//       }
//       stx_return = STX_getBuffer(0, &count);
//       // data mode
//       stx_return = STX_setControl(1, 2);
//       // delay
       int delay = 210000; // random. with logic analyzer find the good number.
//       for(i=0;i<delay;i++);
//       stx_return = STX_setControl(1, 3);
       int tr;
//
//       stx_return = STX_getTR(&tr);
//       stx_return = STX_getBuffer(0, &count);
//       // 5f
//       stx_return = STX_setControl(1, 2);
//       while(tr == 0){
//           STX_getTR(&tr);
//       }
//       stx_return = STX_setControl(1, 3);
//       stx_return = STX_getTR(&tr);
//       stx_return = STX_getBuffer(0, &count);
//       // 5g
//       stx_return = STX_setControl(1, 2);
//       for(i=0;i<delay;i++);
//       stx_return = STX_setControl(1, 3);
//       stx_return = STX_getTR(&tr);
//       stx_return = STX_getBuffer(0, &count);
//       stx_return = STX_getBuffer(1, &underrun);

       // 6a
       // 1
       stx_return = STX_setControl(0, 0);
       stx_return = STX_getBuffer(0, &count);
       stx_return = STX_setEncoder(0,0,1,1);
       stx_return = STX_setFrequency(2300.0);
       // 2
       stx_return = STX_setControl(1, 1);
       // 3
       for(i = 0; i < 10240; i++){
           rand_byte = rand() % 65536; // 16 bit number
           SPISbandTx(&rand_byte);
       }
       stx_return = STX_getBuffer(0, &count);
       stx_return = STX_getBuffer(2, &overrun);
       // probably a breakpoint for 5s
       // 4
       stx_return = STX_setControl(1, 2);
       for(i = 0;i<12*delay;i++);
       // 5
       stx_return = STX_setControl(1, 1);
       // 6
       stx_return = STX_getBuffer(0, &count);
       stx_return = STX_getBuffer(1, &underrun);
       stx_return = STX_setControl(0, 0);
       // 6b
       // 1
       stx_return = STX_setControl(0, 0);
       stx_return = STX_getBuffer(0, &count);
       stx_return = STX_setEncoder(0,0,1,1);
       stx_return = STX_setFrequency(2200.0);
       // 2
       stx_return = STX_setControl(1, 1);
       // 3
       for(i = 0; i < 10240; i++){
           rand_byte = rand() % 65536; // 16 bit number
           SPISbandTx(&rand_byte);
       }
       stx_return = STX_getBuffer(0, &count);
       stx_return = STX_getBuffer(2, &overrun);
       // 4
       stx_return = STX_setControl(1, 2);
       // 5
       int j = 0;
       while (j< 1000){ // not sure if int is long enough
           stx_return = STX_getTR(&tr);
           if (tr == 1){
               for(i = 0; i < 10240; i++){
                   rand_byte = rand() % 65536; // 16 bit number
                   SPISbandTx(&rand_byte);
               }
           }
           j++;
       }
       // 6
       stx_return = STX_getTR(&tr); // Would probably be faster to read TR GPIO line
       while (tr == 0){ // not sure if int is long enough
           stx_return = STX_getTR(&tr);
       }
       stx_return = STX_setControl(1, 1);
       // 7
       stx_return = STX_getBuffer(0, &count);
       stx_return = STX_getBuffer(1, &underrun);
       stx_return = STX_getBuffer(2, &overrun);


       /**/
       // 7a
       uint8_t pwrgd, txl;
       stx_return = STX_getStatus(&pwrgd, &txl);
       // 7b
       stx_return = STX_getTR(&tr);
       // 7c-e
       stx_return = STX_getBuffer(0, &count);
       stx_return = STX_getBuffer(1, &underrun);
       stx_return = STX_getBuffer(2, &overrun);
       // 7f
       uint8_t power;
       stx_return = STX_getPaPower(&power);
       // rest
       sBand_housekeeping hk;
       stx_return = STX_getHK(&hk);
       /**/


       // 6b8
       stx_return = STX_setControl(0, 0);

//       // 8a-b
//       // Use HalcoGen
//       // 8c
//       uint16_t zero_byte = 0;
//       uint16_t one_byte = 1;
//       uint16_t next1 = 500 + rand()%500;;
//       for(i = 0; i < 10240; i++){
//           if (i%next1 == 0){
//               SPISbandTx(&one_byte);// occasionally a 1 bit
//               next1 = 500 + rand()%500;
//           } else SPISbandTx(&zero_byte); // send 0 most of the time
//       }
//       //
//       stx_return = STX_setControl(1, 2); //* Would this empty the buffer?
//       // 8d
//       stx_return = STX_setControl(1, 1);
//       uint16_t allone_byte = 0xFFFFFFFF;
//       uint16_t onezero_byte = 0xFFFFFFFE;
//       for(i = 0; i < 10240; i++){
//           if (i%next1 == 0){
//               SPISbandTx(&onezero_byte);
//               next1 = 500 + rand()%500;
//           } else SPISbandTx(&allone_byte);
//       }
//       stx_return = STX_setControl(1, 2);
//       // 8e
//       stx_return = STX_setControl(1, 1);
//       uint16_t checkers_byte = 0xAAAAAAAA;
//       for(i = 0; i < 10240; i++){
//           SPISbandTx(&checkers_byte);
//       }
//       stx_return = STX_setControl(1, 2);
//       // 8f
//       stx_return = STX_setControl(1, 1);
//       uint16_t block_size = rand() % 2560;
//       uint16_t total_bytes = 0;
//       while (total_bytes < 10240 - block_size){
//           for(i = 0; i < block_size; i++){
//               rand_byte = rand() % 65536;
//               SPISbandTx(&rand_byte);
//           }
//           for(i=0;i<(rand()%10 + 1)*delay;i++);
//           block_size = rand() % 2560;
//       }
//       for(i = 0; i < 10240 - total_bytes; i++){
//           rand_byte = rand() % 65536;
//           SPISbandTx(&rand_byte);
//       }
//       stx_return = STX_setControl(1, 2);
//       // 8g
//       while (j< 60000 * delay){
//           stx_return = STX_setControl(1, 1);
//           for(i = 0; i < 10240; i++){
//               rand_byte = rand() % 65536;
//               SPISbandTx(&rand_byte);
//           }
//           j++;
//           stx_return = STX_setControl(1, 2);// empty the buffer
//       }

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
