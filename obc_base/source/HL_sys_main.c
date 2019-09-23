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
#include "HL_can.h"
#include "HL_esm.h"
#include "HL_system.h"
#include "HL_sys_core.h"
#include <stdio.h>

/* Define Task Handles */
xTaskHandle xTask1Handle;

/* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        /* Taggle HET[1] with timer tick */
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
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

uint8   emacAddress[6U] =   {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
uint32  emacPhyAddress  =   1U;



#define D_COUNT 8

uint32 cnt=0, error =0, tx_done =0;
uint8 tx_data1[D_COUNT] = {1,2,3,4,5,6,7,8};
uint8 tx_data2[D_COUNT] = {11,12,13,14,15,16,17,18};
uint8 tx_data3[D_COUNT] = {21,22,23,24,25,26,27,28};
uint8 tx_data4[D_COUNT] = {31,32,33,34,35,36,37,38};

uint8 rx_data1[D_COUNT] = {0};
uint8 rx_data2[D_COUNT] = {0};
uint8 rx_data3[D_COUNT] = {0};
uint8 rx_data4[D_COUNT] = {0};

uint8 *dptr=0;

int main(void)
{


    canInit();

    canEnableErrorNotification(canREG1);
        canEnableErrorNotification(canREG2);
        canEnableErrorNotification(canREG3);
        canEnableErrorNotification(canREG4);

           canTransmit(canREG1, canMESSAGE_BOX1, (const uint8 *) &tx_data1[0]);
           canTransmit(canREG2, canMESSAGE_BOX1, (const uint8 *) &tx_data2[0]);
           canTransmit(canREG3, canMESSAGE_BOX1, (const uint8 *) &tx_data3[0]);
           canTransmit(canREG4, canMESSAGE_BOX1, (const uint8 *) &tx_data4[0]);


        while(!canIsRxMessageArrived(canREG1,canMESSAGE_BOX1)){
           canGetData(canREG1, canMESSAGE_BOX1, rx_data1);
           canGetData(canREG1, canMESSAGE_BOX1, rx_data2);
           canGetData(canREG1, canMESSAGE_BOX1, rx_data3);
           canGetData(canREG1, canMESSAGE_BOX1, rx_data4);
        }

           int i;
           for(i = 0; i <8; i++){
               if(tx_data1[i] != rx_data1[i]){
                   fprintf(stderr,"can 1, index = %d error\n", i);
                   fprintf(stderr, "tx = %d, rx = %d\n", tx_data1[i], rx_data1[i]);
               }
               else if(tx_data2[i] != rx_data2[i]){
                   fprintf(stderr,"can 2, index = %d error\n", i);
               }
               else if(tx_data3[i] != rx_data3[i]){
                   fprintf(stderr,"can 3, index = %d error\n", i);
               }
               else if(tx_data4[i] != rx_data4[i]){
                   fprintf(stderr,"can 4, index = %d error\n", i);
               }

               else if(i == 7){
                   fprintf(stderr,"Checking tx and rx complete");
               }
           }



           return(0);

}

/* USER CODE END */




/* USER CODE BEGIN (4) */

/* USER CODE END */
