/*
 * cspDMA_EFM32_M3.c
 *
 *  Created on: Jun 20, 2018
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

#include "cspDMA_EFM32_M3.h"
// Descriptor memory space do not do anything
uint8_t cbrAddr[(NUM_DMA_CHANNELS * DESCRIPTOR_SIZE) + BASE_RAW_SIZE];

void cspDMA_Init(uint8_t hprot) {

	/* Make sure DMA clock is enabled prior to accessing DMA module */
	CMU_ClockEnable(cmuClock_DMA, true);

	/* Make sure DMA controller is set to a known reset state */
	DMA_Reset();

	/* Clear/enable DMA interrupts */
	NVIC_ClearPendingIRQ(DMA_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);

	/* Enable bus error interrupt */
	DMA->IEN = DMA_IEN_ERR;

	/*
	 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 * THIS IS A VERY IMPORTANT CODE LINE - BREAK IT AND THE DMA. WILL. NOT. WORK.
	 * The base address for the DMA controller's descriptors MUST be a multiple of the total size
	 * of the memory allocated, according to the datasheet.
	 * ALSO according to the datasheet, the last byte of the address block must be 0.
	 *
	 * There are some ways to achieve this end, one way is to use a specialized malloc, memalign
	 * https://linux.die.net/man/3/memalign
	 * However, it is obsolete, uses different libraries, etc.
	 *
	 * Another way is to allocate more than you need, but enough so that you can be guaranteed to find an appropriate address
	 * https://stackoverflow.com/questions/227897/how-to-allocate-aligned-memory-only-using-the-standard-library
	 * I will let the most popular answer explain how it works.
	 *
	 * My method: Allocate 256 + (16 * NUM_DMA_CHANNELS) statically
	 * With this you will have a chunk of memory guaranteed to have a block size that is
	 * 16 * NUM_DMA_CHANNELS bytes and whose base address ends in 0x00.
	 *
	 */
	DMA->CTRLBASE = (((uintptr_t)cbrAddr + ALIGN_MASK) & ~ALIGN_MASK);

	/* END OF VERY IMPORTANT CODE BLOCK THE REST IS COPY/PASTA FROM SILICON LABS! */

	/* Configure and enable the DMA controller */
	DMA->CONFIG = (hprot << _DMA_CONFIG_CHPROT_SHIFT)
                | DMA_CONFIG_EN;

	/*
	* Changing the priority of DMA IRQ to use FreeRTOS functions.
	* It must be numerically equal to or greater than configMAX_SYSCALL_INTERRUPT_PRIORITY
	* defined in FreeRTOSConfig.h
	* Currently, that is set to 5.
	* I make the DMA have a higher priority than the I2C interrupt.
	* That, originally, is how it worked.
	*/
	NVIC_SetPriority(DMA_IRQn, DMA_INT_PRIO_LEVEL);
}

