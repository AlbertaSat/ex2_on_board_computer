/*
 * cspDMA_EFM32_M3.h
 *
 *  Created on: Jun 19, 2018
 *      Author: kgmills
 *
 *      File contains DMA configuration information
 *      for all used peripherals.
 *      Such as defining number of channels, the channels used
 *      by each peripheral, and also that pesky memory management stuff.
 *
 *      See Silicon Labs Application Note #0013 for more information
 *      https://www.silabs.com/documents/public/application-notes/AN0013.pdf
 *
 */

#ifndef CSPDMA_EFM32_M3_H_
#define CSPDMA_EFM32_M3_H_

#include "em_dma.h"
#include "em_cmu.h"

/*
 * INITIALIZATION CONSTANTS
 *
 * NUM_DMA_CHANNELS - Max 12 primary, 12 alternate (which I'm not touching)
 * 	Currently: 2, for I2C Tx/Rx
 * DESCRIPTOR_SIZE - Size of a descriptor block needed to be allocated.
 * BASE_RAW_SIZE: Base size of address block that will include an address which ends in 0x00.
 * ALIGN_MASK: Alignment mask for the descriptor memory chunk.
 * CSP_HPROT: Memory protection variable that is typically unset.
 * controlBlock - Array used. Will be aligned on init.
 */
#define NUM_DMA_CHANNELS				2
#define DESCRIPTOR_SIZE					16
#define BASE_RAW_SIZE					256
#define ALIGN_MASK						255
#define CSP_HPROT						0
#define DMA_INT_PRIO_LEVEL				5
static DMA_CB_TypeDef 					dmaCB;

/*
 * CONSTANTS USED FOR PHYSICAL LAYER INTERFACES AND LOGICAL OPERATIONS
 *
 * I2C Specific Constraints - Used by cspDMA_EFM32_M3.c/h
 * DMA_CHANNEL_I2C_TX/RX is the channel used, between 0 and 11
 *
 * DMA_ENABLE_I2C_TX/Rx is the ENABLE/DISABLE bit for the DMA->CHENS register.
 * 	It is a wrapper for another variable, which should be of the form
 * 	DMA_CHENS_CH#ENS where # is the channel number
 *
 * CHANNEL_MULT_OFFSET is the hex value which is to be multiplied by the channel number
 * 	and then added to the base address of the DMA->CTRLBASE register to reach
 * 	the descriptor base address for that channel
 *
 * CTRL_ADD_OFFSET is the offset used for accessing the CTRL field of a channel's descriptor
 * 	added to the base address
 *
 * TRANS_REMAIN_MASK: AND this with the value of the CTRL field for a descriptor to clear all bits
 * 	which are not part of the n_minus_1 field (telling you how many transfers are left)
 *
 * TRANS_REMAIN_SHIFT: AFTER ANDing CTRL with TRANS_REMAIN_MASK, right shift the result (>>) by
 * 	this value to get the # of transfers that have yet to be completed.
 * 	Then, based on the value of transfers requested, you can see how many remain.
 *
 *	For details on how to recover the number of bytes that has been received,
 * 	please visit the Silicon Labs Reference Manual for the
 * 	EFM32 Giant Gecko with the ARM Cortex-M3 processor.
 * 	https://www.silabs.com/documents/public/reference-manuals/EFM32GG-RM.pdf
 * 	Chapter 8 - DMA; Pages 64-66 on the n_minus_1 data field
 *
 */
#define DMA_CHANNEL_I2C_TX 				0
#define DMA_ENABLE_I2C_TX  				DMA_CHENS_CH0ENS
#define DMA_CHANNEL_I2C_RX				1
#define DMA_ENABLE_I2C_RX				DMA_CHENS_CH1ENS
#define DMA_COMPLETE_I2C_TX   			DMA_IFS_CH0DONE
#define DMA_COMPLETE_I2C_RX				DMA_IFS_CH1DONE
#define CHANNEL_MULT_OFFSET				0x10
#define CTRL_ADD_OFFSET					0x08
#define TRANS_REMAIN_MASK				0x00003FF0
#define TRANS_REMAIN_SHIFT				4

/***************************************************************************//**
 * @brief
 *   Initializes DMA controller. Copied from SiLabs code from em_dma.c and modified so that it can work
 *   For a variable number of active DMA channels instead of assuming all are active.
 *
 * @details
 *   This function will reset and prepare the DMA controller for use. Although
 *   it may be used several times, it is normally only used during system
 *   init. If reused during normal operation, notice that any ongoing DMA
 *   transfers will be aborted. When completed, the DMA controller is in
 *   an enabled state.
 *
 * @note
 *   Must be invoked before using the DMA controller.
 *
 * @param[in] init
 *   Pointer to a structure containing DMA init information.
 ******************************************************************************/
void cspDMA_Init(uint8_t hprot);

#endif /* CSPDMA_EFM32_M3_H_ */
