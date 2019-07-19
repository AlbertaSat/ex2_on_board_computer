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
#include "HL_gio.h"
#include "HL_het.h"
#include "FreeRTOS.h"
#include "os_task.h"

//File System includes
#include <redfs.h>
#include <redposix.h>
#include <redfse.h>
#include <redconf.h>
#include <redvolume.h>
#include <redtests.h>



/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */

// Arguments to mount redfs filesystems
struct redfs_args
{
    const char *fspec;  // Name of device to mount
    unsigned int flags; // REDFSMNT_* flags
};

#define REDFSMNT_FMTONCE 0x1U   // format if volume cannot be mounted
#define REDFSMNT_FMTALWAYS 0x2U // format always

/* Priorities at which the tasks are created. */
#define mainCHECK_TASK_PRIORITY (configMAX_PRIORITIES - 2)
#define mainQUEUE_POLL_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainSEM_TEST_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainBLOCK_Q_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainCREATOR_TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define mainINTEGER_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainGEN_QUEUE_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainFLOP_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainQUEUE_OVERWRITE_PRIORITY (tskIDLE_PRIORITY)

#define mainTIMER_TEST_PERIOD (50)

void vAssertCalled(
unsigned long ulLine, const char * const pcFileName
)
{
    //printf("error line: %lu in file: %s", ulLine, pcFileName);
    gioToggleBit(gioPORTB,6);

}


extern void main_blinky( void );
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
    gioInit();

    gioSetDirection(gioPORTB, 0xFFFFFFFF);
    main_blinky();

    int32_t iErr;
    const char *pszVolume0 = gaRedVolConf[0].pszPathPrefix;
    const char *pszVolume1 = gaRedVolConf[1].pszPathPrefix;

    //iErr = red_init(); // TODO: Line currently causes errors
//    fprintf("%d\n", iErr);
//    if (iErr == -1)
//    {
//        //fprintf(stderr, "Unexpected error %d from red_init()\n", (int)red_errno);
//        gioToggleBit(gioPORTB,6);
//        exit(red_errno);
//    }
//
//    iErr = red_format(pszVolume0);
//    if (iErr == -1)
//    {
//        //fprintf(stderr, "Unexpected error %d from red_format()\n", (int)red_errno);
//        gioToggleBit(gioPORTB,6);
//        exit(red_errno);
//    }
//
//    iErr = red_mount(pszVolume0);
//    if (iErr == -1)
//    {
//        //fprintf(stderr, "Unexpected error %d from red_mount()\n", (int)red_errno);
//        gioToggleBit(gioPORTB,6);
//        exit(red_errno);
//    }

    /*iErr = red_format(pszVolume1);
    if (iErr == -1)
    {
        //fprintf(stderr, "Unexpected error %d from red_format()\n", (int)red_errno);
        gioToggleBit(gioPORTB,6);
        exit(red_errno);
    }

    iErr = red_mount(pszVolume1);
    if (iErr == -1)
    {
        //fprintf(stderr, "Unexpected error %d from red_mount()\n", (int)red_errno);
        gioToggleBit(gioPORTB,6);
        exit(red_errno);
    }
    */


    //FileSystem Tasks
//    vStartFSWriteTask(mainCREATOR_TASK_PRIORITY);
//
//
//    vTaskStartScheduler();

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
