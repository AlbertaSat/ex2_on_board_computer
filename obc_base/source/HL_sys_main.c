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

int main(void)
{
/* USER CODE BEGIN (3) */
    _enable_IRQ();
    InitIO();
    i2cInit();


    int return_value = -2;
    uint8 sample_scw[12] = {0};
    return_value = UHF_genericRead(0, sample_scw);

//    uint32 freq, uptime, transmitted_pckts = 10, received_pckts = 10, received_pckts_crc_err = 10, pipe_t, beacon_t, audio_t;
//    return_value = UHF_genericRead(1, &freq);
//    return_value = UHF_genericRead(2, &uptime);
//    return_value = UHF_genericRead(3, &transmitted_pckts);
//    return_value = UHF_genericRead(4, &received_pckts);
//    return_value = UHF_genericRead(5, &received_pckts_crc_err);
//    return_value = UHF_genericRead(6, &pipe_t);
//    return_value = UHF_genericRead(7, &beacon_t);
//    return_value = UHF_genericRead(8, &audio_t);
//    BROKEN: return_value = UHF_genericRead(10, &temp);
//    uint8 status;
//    return_value = UHF_genericRead(244, &status);
//    uhf_configStruct source_callsign, dest_callsign, morse_callsign, midi_beacon, beacon_msg;
//    return_value =  UHF_genericRead(245, &dest_callsign);
//    return_value =  UHF_genericRead(246, &source_callsign);
//    return_value =  UHF_genericRead(247, &morse_callsign);
//    return_value =  UHF_genericRead(248, &midi_beacon);
//    uint8 version[4];
//    return_value =  UHF_genericRead(249, &version);
//    uint16 payload_size;
//    return_value = UHF_genericRead(250, &payload_size);
//    return_value =  UHF_genericRead(251, &beacon_msg);
//    uhf_framStruct fram_data;
//    fram_data.add = 0x24001;
//    return_value = UHF_genericRead(253, &fram_data);

    /* Test 3*/
       /* 3a */
       uint32 freq = 0;
//       return_value = UHF_genericRead(1, &freq);
       uint8 reset_scw[12] = {0,3,0,3,0,0,0,0,0,0,1,1};
       return_value = UHF_genericWrite(0, reset_scw);
//       return_value = UHF_genericRead(1, &freq);

       /* 3b */
       uint8 set_scw[12]= {1,0,0,1,1,0,0,0,1,0};
       uint8 get_scw[12]= {0};
       return_value = UHF_genericWrite(0, &set_scw);
       return_value = UHF_genericRead(0, &get_scw);
       /* 3c */
//       float set_freq = 435.78; //* Why uint32 in EH code?
//       float get_freq;
//       UHF_genericWrite(1, &set_freq);
//       return_value = UHF_genericRead(1, &get_freq);
//       /* 3d */
//       uint16 set_pipe_t = 30; //* Shouldn't be uint8? Read and write are inconsistent.
//       uint16 get_pipe_t;
//       return_value = UHF_genericWrite(6, &set_pipe_t);
//       return_value = UHF_genericRead(6, &get_pipe_t);
//       /* 3e */
//       uint16 set_beacon_t = 30;
//       uint16 get_beacon_t;
//       return_value = UHF_genericWrite(7, &set_beacon_t);
//       return_value = UHF_genericRead(7, &get_beacon_t);
//       /* 3f */
//       uint16 set_audio_t = 60;
//       uint16 get_audio_t;
//       return_value = UHF_genericWrite(8, &set_audio_t);
//       return_value = UHF_genericRead(8, &get_audio_t);
//       /* 3g & 3i*/
//       uint8 confirm = 1;
//       uint8 low_status;
//       return_value = UHF_genericWrite(244, &confirm);
//       return_value = UHF_genericRead(244, &low_status);
//       //* Does it come out of low_power mode or we need a reset?
//       /* 3h */
//       uhf_configStruct set_dest_callsign, get_dest_callsign;
//       set_dest_callsign.len = 6;
//       strncpy(set_dest_callsign.message, 'VE6UAB', set_dest_callsign.len);
//       return_value = UHF_genericWrite(245, &set_dest_callsign);
//       return_value = UHF_genericRead(245, &get_dest_callsign);
//       /* 3j */
//       uhf_configStruct set_src_callsign, get_src_callsign;
//       set_src_callsign.len = 6;
//       strncpy(set_src_callsign.message, 'EX2UAB', set_src_callsign.len);
//       return_value = UHF_genericWrite(246, &set_src_callsign);
//       return_value = UHF_genericRead(246, &get_src_callsign);
//       /* 3k */
//       uhf_configStruct set_morse_callsign, get_morse_callsign;
//       set_dest_callsign.len = 24;
//       strncpy(set_dest_callsign.message,'. -..- ..--- ..- .- -...', set_dest_callsign.len);
//       return_value =UHF_genericWrite(247, &set_morse_callsign);
//       return_value = UHF_genericRead(247, &get_morse_callsign);
//       /* 3l */
//       uhf_configStruct set_midi_beacon, get_midi_beacon;
//       set_midi_beacon.len = 9;
//       strncpy(set_midi_beacon.message, '67H69H71H', set_midi_beacon.len); //* Discuss the length in the test plan
//       return_value = UHF_genericWrite(248, &set_midi_beacon);
//       return_value = UHF_genericRead(248, &get_midi_beacon);
//       /* 3m */
//       uhf_configStruct set_beacon_msg, get_beacon_msg;
//       set_beacon_msg.len = 16;
//       strncpy(set_beacon_msg.message, 'Hello AlbertaSat', set_beacon_msg.len);
//       return_value = UHF_genericWrite(251, &set_beacon_msg);
//       return_value = UHF_genericRead(251, &get_beacon_msg);
//       /* 3n */
//       uint8 i2c_addr = 0x23;
//       return_value = UHF_genericWrite(252, &i2c_addr);
//       //* Would we need to update our I2C I/O function to send the next command?
//       /* 3o */
//       uhf_framStruct set_fram_data, get_fram_data;
//       set_fram_data.add = 0x24001; //* uint32: 147457
//       strcpy(set_fram_data.data, '0123456789ABCDEF');
//       return_value = UHF_genericWrite(253, &set_fram_data);
//       return_value = UHF_genericRead(253, &get_fram_data);
//
//       /* Additional tests */
//       /* Read some info */
//       uint32 secure_key;
//       return_value = UHF_genericRead(255, &secure_key);
//       uint16 payload_size;
//       return_value = UHF_genericRead(16, &payload_size);
//       /* Other SCW tests*/
//       /* Echo */
//       set_scw[7] = 1; //* or [4]
//       return_value = UHF_genericWrite(0, &set_scw);
//       return_value = UHF_genericRead(0, &get_scw);
//       /* PIPE */

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
