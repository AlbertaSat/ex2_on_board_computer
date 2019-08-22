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
#include "HL_mibspi.h"
#include "HL_esm.h"
#include "HL_sys_core.h"
#include "HL_system.h"

#include <redfs.h>
#include <redposix.h>
#include <redfse.h>
#include <redconf.h>
#include <redvolume.h>
#include <redtests.h>

#include <stdio.h>
#include <stdlib.h>

/* Define Task Handles */
xTaskHandle xTask1Handle;

/* Task1 */
void vTask1(void *pvParameters)
{

        int32_t iErr;
        const char *pszVolume0 = gaRedVolConf[0].pszPathPrefix;

        iErr = red_init();
        if (iErr == -1)
        {
            fprintf(stderr, "Unexpected error %d from red_init()\n", (int)red_errno);
            exit(red_errno);
        }

        iErr = red_format(pszVolume0);
        if (iErr == -1)
        {
            fprintf(stderr, "Unexpected error %d from red_format()\n", (int)red_errno);
            exit(red_errno);
        }

        iErr = red_mount(pszVolume0);
        if (iErr == -1)
        {
            fprintf(stderr, "Unexpected error %d from red_mount()\n", (int)red_errno);
            exit(red_errno);
        }
}

void vTask2(void *pvParameters)
{


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
#define D_COUNT  8

uint32 cnt=0, error =0, tx_done =0;
uint16 tx_data1[D_COUNT] = {1,2,3,4,5,6,7,8};
uint16 tx_data2[D_COUNT] = {11,12,13,14,15,16,17,18};
uint16 tx_data3[D_COUNT] = {21,22,23,24,25,26,27,28};
uint16 tx_data4[D_COUNT] = {31,32,33,34,35,36,37,38};
uint16 tx_data5[D_COUNT] = {41,42,43,44,45,46,47,48};


uint16 rx_data1[D_COUNT] = {0};
uint16 rx_data2[D_COUNT] = {0};
uint16 rx_data3[D_COUNT] = {0};
uint16 rx_data4[D_COUNT] = {0};
uint16 rx_data5[D_COUNT] = {0};
/* USER CODE END */

uint8	emacAddress[6U] = 	{0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */




    /* Create Task 1 */
    if (xTaskCreate(vTask1,"Task1", 1024, NULL, 1, &xTask1Handle) != pdTRUE)
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
/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static – otherwise they will be allocated on
the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
    /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    /* Pass out the array that will be used as the Idle task’s stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


/* can interrupt notification */
void mibspiGroupNotification(mibspiBASE_t *mibspi, uint32 group)
{

    uint16 * data;

    /* node 1 - transfer request */
     if(mibspi==mibspiREG1)
     {
         data = &rx_data1[0];
     }
     if(mibspi==mibspiREG2)
     {
         data = &rx_data2[0];
     }
     if(mibspi==mibspiREG3)
     {
         data = &rx_data3[0];
     }
     if(mibspi==mibspiREG4)
     {
         data = &rx_data4[0];
     }
     if(mibspi==mibspiREG5)
     {
         data = &rx_data5[0];
     }

     mibspiGetData(mibspi, group, data);
}
/* USER CODE END */
