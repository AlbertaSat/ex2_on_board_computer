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

void UHF_testScript(void);
void T2T_testing(void);

int main(void)
{
/* USER CODE BEGIN (3) */
    _enable_IRQ();
    InitIO();

    gioSetBit(hetPORT2, 22, 1); //Enables UHF transceiver
    gioSetBit(hetPORT2, 23, 1); //Enables S-band transmitter
    gioSetBit(hetPORT2, 21, 1); //Ensures the transmitter is not being reset

    // Delay to allow the board to initialize after enabling
    int j_delay;
    for(j_delay = 0; j_delay < 0x800000; j_delay++);

    UHF_testScript();

    T2T_testing();

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

void T2T_testing(void){
    /* Transceiver to Transceiver (T2T) comms testing */
    //    // Enter PIPE mode (I2C command)
    //    uint8_t T2T_test_scw[12] = {0, UHF_UARTBAUD_115200, 0, UHF_RFMODE5, 0, 0, UHF_PIPE_ON, 0, 0, 0, 1, 1};
    //    uint8_t uhf_return;
    //    uhf_return = UHF_genericWrite(0, );
    //    // Wait to receive 1 byte over UART
    //    uint8_t received_message = 0;
    //    received_message = sciReceiveByte(sciREG2);
    //    UHF_genericRead(0, initial_scw);
}

void UHF_testScript(void){

    uint8_t return_value;

    /* Test Section 2 - Read Commands*/
    uint32 freq, uptime, transmitted_pckts, received_pckts, received_pckts_crc_err, pipe_t, beacon_t, audio_t;
    UHF_genericRead(1, &freq);
    UHF_genericRead(2, &uptime);
    UHF_genericRead(3, &transmitted_pckts);
    UHF_genericRead(4, &received_pckts);
    UHF_genericRead(5, &received_pckts_crc_err);
    UHF_genericRead(6, &pipe_t);
    UHF_genericRead(7, &beacon_t);
    UHF_genericRead(8, &audio_t);
    float temp;
    UHF_genericRead(10, &temp);
    uint8 status;
    UHF_genericRead(244, &status);


    uhf_configStruct source_callsign, dest_callsign, morse_callsign, midi_beacon, beacon_msg;
    return_value =  UHF_genericRead(245, &dest_callsign);
    return_value =  UHF_genericRead(246, &source_callsign);
    return_value =  UHF_genericRead(247, &morse_callsign);
    return_value =  UHF_genericRead(248, &midi_beacon);
    uint8 version[4];
    return_value =  UHF_genericRead(249, &version);
    uint16 payload_size;
    return_value = UHF_genericRead(250, &payload_size);
    return_value =  UHF_genericRead(251, &beacon_msg);

    uhf_framStruct fram_data;
    fram_data.add = 0x24001;
    return_value = UHF_genericRead(253, &fram_data);

    /* Test Section 3 - Write Commands - modified */
           /* 3a - modified */
        return_value = UHF_genericRead(1, &freq);
        uint8 set_scw[12]= {0,3,0,5,0,0,0,0,0,0,1,1};
        uint8 get_scw[12]= {0};
        return_value = UHF_genericWrite(0, set_scw);
        return_value = UHF_genericRead(0, get_scw);

           /* 3c */
           uint32 set_freq = 437875000; //* Why uint32 in EH code?
           uint32 get_freq;
           UHF_genericWrite(1, &set_freq);
           return_value = UHF_genericRead(1, &get_freq);
           /* 3d */
           uint16 set_pipe_t = 35; //* Shouldn't be uint8? Read and write are inconsistent.
           uint32 get_pipe_t;
           return_value = UHF_genericWrite(6, &set_pipe_t);
           return_value = UHF_genericRead(6, &get_pipe_t);
           /* 3e */
           uint16 set_beacon_t = 65;
           uint32 get_beacon_t;
           return_value = UHF_genericWrite(7, &set_beacon_t);
           return_value = UHF_genericRead(7, &get_beacon_t);
           set_scw[5]= 1; // beacon on
           return_value = UHF_genericWrite(0, set_scw);
           return_value = UHF_genericRead(0, get_scw);
           set_scw[5]= 0; // beacon off
           return_value = UHF_genericWrite(0, set_scw);
           return_value = UHF_genericRead(0, get_scw);
           /* 3f */
           uint16 set_audio_t = 0;
           uint32 get_audio_t;
           return_value = UHF_genericWrite(8, &set_audio_t);
           return_value = UHF_genericRead(8, &get_audio_t);
           /* 3g & 3i*/
           uint8 confirm = 1;
           uint8 low_status;
           return_value = UHF_genericWrite(244, &confirm);
           return_value = UHF_genericRead(244, &low_status);
           //* Does it come out of low_power mode or we need a reset?
           /* 3h */
             uhf_configStruct set_dest_callsign, get_dest_callsign;
             set_dest_callsign.len = 6;
             set_dest_callsign.message[0] = 'V';
             set_dest_callsign.message[1] = 'E';
             set_dest_callsign.message[2] = '6';
             set_dest_callsign.message[3] = 'U';
             set_dest_callsign.message[4] = 'A';
             set_dest_callsign.message[5] = 'B';
             return_value = UHF_genericWrite(245, &set_dest_callsign);
             return_value = UHF_genericRead(245, &get_dest_callsign);

             /* 3j */
             int i;
           uhf_configStruct set_src_callsign, get_src_callsign;
           set_src_callsign.len = 6;
           uint8_t src_callsign[6] = "EX2UAB";
           for (i=0;i<set_src_callsign.len; i++){
               set_src_callsign.message[i] = src_callsign[i];
           }
           return_value = UHF_genericWrite(246, &set_src_callsign);
           return_value = UHF_genericRead(246, &get_src_callsign);
           /* 3k - modified */
           uhf_configStruct set_morse_callsign, get_morse_callsign;
           set_morse_callsign.len = 24;
           uint8_t test_morse_callsign[24] = ". -..- ..--- ..- .- -...";
           for (i=0;i<set_morse_callsign.len; i++){
               set_morse_callsign.message[i] = test_morse_callsign[i];
           }
           return_value = UHF_genericWrite(247, &set_morse_callsign);
           return_value = UHF_genericRead(247, &get_morse_callsign);
           /* 3l */


           uhf_configStruct *set_midi_beacon = (uhf_configStruct *)pvPortMalloc(sizeof(uhf_configStruct));
           uhf_configStruct *get_midi_beacon = (uhf_configStruct *)pvPortMalloc(sizeof(uhf_configStruct));

           //* Discuss the length in the test plan. The full MIDI crashes.
           set_midi_beacon->len = 14;
           uint8_t midi_callsign[42] = "67H69H71H67H67H69H71H67H71H72H74W71H72H74W";
           for (i=0;i<(3*set_midi_beacon->len); i++){
               set_midi_beacon->message[i] = midi_callsign[i];
           }
           return_value = UHF_genericWrite(248, set_midi_beacon);
           return_value = UHF_genericRead(248, get_midi_beacon);
           vPortFree(set_midi_beacon);
           vPortFree(get_midi_beacon);

           /* 3m */
             uhf_configStruct *set_beacon_msg = (uhf_configStruct *)pvPortMalloc(sizeof(uhf_configStruct));
             uhf_configStruct *get_beacon_msg = (uhf_configStruct *)pvPortMalloc(sizeof(uhf_configStruct));
             set_beacon_msg->len = 16;
             uint8_t beacon_callsign[16] = "Hello AlbertaSat";
             for (i=0;i<set_beacon_msg->len; i++){
                 set_beacon_msg->message[i] = beacon_callsign[i];
             }
             return_value = UHF_genericWrite(251, set_beacon_msg);
             return_value = UHF_genericRead(251, get_beacon_msg);
           /* 3n */
//           uint8 i2c_addr = 0x22;
//           return_value = UHF_genericWrite(252, &i2c_addr);
           //* Would we need to update our I2C I/O function to send the next command?
           /* 3o */
            uhf_framStruct *set_fram_data = (uhf_framStruct *)pvPortMalloc(sizeof(uhf_framStruct));
            uhf_framStruct *get_fram_data = (uhf_framStruct *)pvPortMalloc(sizeof(uhf_framStruct));
           set_fram_data->add = 0x24001; //* uint32: 147457
           get_fram_data->add = 0x24001;
           uint8_t test_fram_data[16] = "0123456789ABCDEF";
           for (i=0;i<16; i++){
               set_fram_data->data[i] = test_fram_data[i];
           }
           return_value = UHF_genericWrite(253, set_fram_data);
           return_value = UHF_genericRead(253, get_fram_data);

           /* Additional tests */
           /* Read some info */
           uint32 secure_key;
           return_value = UHF_genericRead(255, &secure_key);
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
