/*
 * SharedMemory.c
 *
 *  Created on: Jun 5, 2018
 *      Author: kgmills
 *
 *      Implements shared memory blocks of a fixed size for FreeRTOS
 *      Using a queue.
 *
 *      REQUIREMENTS: Your FreeRTOS Memory Configuration File
 *      heap_Q.c, must be such that Q > 1.
 *
 */

#include "SharedMemory.h"

/*
 * @brief Create the shared memory block
 * @arg blockSize - Size_t block of memory e.g. for integers sizeof(int)
 * @arg numBlocks - Number of blocks within the shared memory
 * @return Handle to the shared memory block if successful, NULL otherwise
 */
SharedMem_t xSharedMemoryCreate(size_t blockSize, int16_t numBlocks) {

	// Make handle, return NULL if not possible.
	SharedMem_t shMem = xQueueCreate(numBlocks, sizeof(void*));

	if (shMem == NULL) {
		return NULL;
	}

	// We want an array of the pointers, because say if a queue send fails we want to free them all.
	void** initPointers = pvPortMalloc(sizeof(void*) * numBlocks);

	// Iterate over number of blocks.
	for (int16_t i = 0; i < numBlocks; i++) {
		initPointers[i] = pvPortMalloc(sizeof(uint8_t) * blockSize); // Malloc, keep track of pointer now and in future loops
		if (xQueueSend(shMem, &initPointers[i], QUEUE_TO) != pdTRUE) { // Put in queue
			vQueueDelete(shMem); // If that doesn't work, delete the queue
			for (int16_t j = 0; j <= i; j++) {
				vPortFree(initPointers[j]); // Free all blocks up to that point
			}
			vPortFree(initPointers);
			return NULL; // Fail
		}
	}
	vPortFree(initPointers);
	return shMem; // Return handle otherwise
}

SharedMem_t xSharedMemoryCreateStatic(void** staticArrayPtr, int16_t numBlocks) {

	// Make handle, return NULL if not possible
	SharedMem_t shMem = xQueueCreate(numBlocks, sizeof(void*));

	if (shMem == NULL) {
		return NULL;
	}

	// Iterate over blocks, adding their pointers to the queue
	for (int16_t i = 0; i < numBlocks; i++) {
		if (xQueueSend(shMem, &staticArrayPtr[i], QUEUE_TO) != pdTRUE) {
			vQueueDelete(shMem);
			return NULL; // If one fails, they all fail.
		}
	}

	return shMem;
}

/*
 * @brief Get the pointer to a block of memory, a cast is required.
 * @arg shMem - Handle of the memory block
 * @return Pointer to block of memory
 */
void *pSharedMemGet(SharedMem_t shMem) {
	void* memBlock;
	if (xQueueReceive(shMem, &memBlock, QUEUE_TO) == pdFALSE) {return pdFALSE;}
	return memBlock;
}

/*
 * @brief pSharedMemGet from an ISR
 * @arg shMem - Handle to the shared memory
 * @arg pxHTW Pointer to higher priority task woken.
 * @return Pointer to block of memory
 */
void *pSharedMemGetFromISR(SharedMem_t shMem, BaseType_t *pxHTW) {

	void* memBlock;
	if (xQueueReceiveFromISR(shMem, &memBlock, pxHTW) == pdFALSE) {return pdFALSE;}
	return memBlock;
}

/*
 * @brief Put a piece of shared memory back into the block
 * @arg shMem - Handle of the memory block
 * @arg memBlock - Pointer to the block to be placed back
 * @return pdTRUE if successful, pdFALSE if not.
 */
BaseType_t xSharedMemPut(SharedMem_t shMem, void *memBlock) {
	return xQueueSend(shMem, &memBlock, QUEUE_TO);
}

/*
 * @brief xSharedMemPut from an ISR
 * @arg shMem - Handle of the memory block
 * @arg pxHTW - Pointer to higher priority task woken
 * @return pdTRUE if successful, pdFALSE if not.
 */
BaseType_t xSharedMemPutFromISR(SharedMem_t shMem, void *memBlock, BaseType_t *pxHTW) {
	return xQueueSendFromISR(shMem, &memBlock, pxHTW);
}
