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

    gioSetBit(hetPORT2, 22, 1); // Enables the UHF transceiver

    int return_value = -2;
    uint8 sample_scw[12] = {0};
    return_value = UHF_genericRead(0, sample_scw);

//    // 4a
//    uint8 rand_scw[12] = {0};
//    uint16 rand_pipe_t, rand_beacon_t, rand_audio_t;
//    uint32 rand_freq;
//    uhf_configStruct *rand_dest_callsign = (uhf_configStruct *)pvPortMalloc(sizeof(uhf_configStruct));
//    uhf_configStruct *rand_source_callsign = (uhf_configStruct *)pvPortMalloc(sizeof(uhf_configStruct));
//    uhf_configStruct *rand_beacon_msg = (uhf_configStruct *)pvPortMalloc(sizeof(uhf_configStruct));
//    srand((unsigned) time(NULL));
//    int k;
//    for(k=0;k<12;k++){
//        rand_scw[k] = sample_scw[k];
//    }
//
//    int i;
//    for(i=0; i < 10; i++){
//        int r = rand();
//        rand_scw[3] = r % 8;
//        r = rand();
//        rand_scw[4] = r % 2;
//        r = rand();
//        rand_scw[5] = r % 2;
//
//        r = rand();
//        rand_freq = (r % 3000) * 1000 + 435000000;
//
//        r = rand();
//        rand_pipe_t = (r % 255) + 1;
//        r = rand();
//        rand_beacon_t = (r % 65535) + 1;
//        r = rand();
//        rand_audio_t = r % 65536;
//
//        int j;
//        rand_dest_callsign->len = 6;
//        rand_source_callsign->len = 6;
//        for(j=0;j<6;j++){
//            r = rand();
//            rand_dest_callsign->message[j] = (r % 95) + 32;
//            r = rand();
//            rand_source_callsign->message[j] = (r % 95) + 32;
//        }
//
//        r = rand();
//        rand_beacon_msg->len = (r % 97) + 1;
//        for(j=0; j < rand_beacon_msg->len; j++){
//            r = rand();
//            rand_beacon_msg->message[j] = (r % 95) + 32;
//        }
//
//        UHF_genericWrite(0, rand_scw);
//        UHF_genericWrite(1, &rand_freq);
//        UHF_genericWrite(6, &rand_pipe_t);
//        UHF_genericWrite(7, &rand_beacon_t);
//        UHF_genericWrite(8, &rand_audio_t);
//        UHF_genericWrite(245, rand_dest_callsign);
//        UHF_genericWrite(246, rand_source_callsign);
//        UHF_genericWrite(251, rand_beacon_msg);
//    }
//
//    uint8 confirm = 1;
//    UHF_genericWrite(9, &confirm);
//
//    uint32 sample_pipe_t, sample_beacon_t, sample_audio_t;
//
//    UHF_genericRead(6, &sample_pipe_t);
//    UHF_genericRead(7, &sample_beacon_t);
//    UHF_genericRead(8, &sample_audio_t);
//    UHF_genericRead(245, rand_dest_callsign);
//    UHF_genericRead(246, rand_source_callsign);

    //4b
//    uint8 en_uhf_pin = 22; // ICD: H2_7: GPIO6 | Header pin assignment: en_uhf: H2_7 | board_io_test: GPIO6:gioSetBit(hetPORT2, 22, value);
//    gioInit();
//    gioSetDirection(hetPORT2, 0xFFFFFFEA);
//    gioToggleBit(hetPORT2, en_uhf_pin);
//    gioToggleBit(hetPORT2, en_uhf_pin);
//    gioToggleBit(hetPORT2, en_uhf_pin);

//    gioSetBit(hetPORT2, en_uhf_pin, 0);
//    gioSetBit(hetPORT2, en_uhf_pin, 1);
//    gioSetBit(hetPORT2, en_uhf_pin, 0);


//    //4c
//    // Generating 100 unique random locations in 1000 possible locations
//    uint32 *address_ls = (uint32 *)pvPortMalloc(100*sizeof(uint32));
//    uint8 num = 100;
//    uint32 max_add = 1000;
//    uint32 i,j=0;
//    for (i=0; i<max_add && j<num; ++i){
//        int ri = max_add - i;
//        int rj = num - j;
//        if (rand() % ri < rj)
//            *(address_ls + (j++)) = i;
//    }
//
//    uint32 addrress0 = 0x24001;
//    uhf_framStruct *set_fram_data = (uhf_framStruct *)pvPortMalloc(sizeof(uhf_framStruct));
//    uhf_framStruct *get_fram_data = (uhf_framStruct *)pvPortMalloc(sizeof(uhf_framStruct));
//
//    uint8 d;
//    for(d=0; d<num; d++){
//        set_fram_data->add = addrress0 + 16*address_ls[d];
//        uint8 square;
//        for (square=0; square<16; square+=2){
//            set_fram_data->data[square] = 0xFF;
//            set_fram_data->data[square+1] = 0x00;
//        }
//        return_value = UHF_genericWrite(253, set_fram_data);
//    }
//
//    for(d=0; d<num; d++){
//        get_fram_data->add = addrress0 + 16*address_ls[d];
//        return_value = UHF_genericRead(253, get_fram_data);
//    }
//    vPortFree(set_fram_data);
//    vPortFree(get_fram_data);
//    vPortFree(address_ls);

    //4e
    /* 3c */
       uint32 set_freq = 435150000;
       uint32 get_freq;
       UHF_genericWrite(1, &set_freq);
       return_value = UHF_genericRead(1, &get_freq);
       /* 3d */
       uint16 set_pipe_t = 30;
       uint32 get_pipe_t;
       return_value = UHF_genericWrite(6, &set_pipe_t);
       return_value = UHF_genericRead(6, &get_pipe_t);


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
