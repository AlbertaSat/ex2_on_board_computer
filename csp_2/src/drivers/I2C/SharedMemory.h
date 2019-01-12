/*
 * SharedMemory.h
 *
 *  Created on: Jun 5, 2018
 *      Author: kgmills
 */

#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"

#define QUEUE_TO				0

typedef QueueHandle_t SharedMem_t;

SharedMem_t xSharedMemoryCreate(size_t blockSize, int16_t numBlocks);

SharedMem_t xSharedMemoryCreateStatic(void** staticArrayPtr, int16_t numBlocks);

void *pSharedMemGet(SharedMem_t shMem);
void *pSharedMemGetFromISR(SharedMem_t shMem, BaseType_t *pxHTW);

BaseType_t xSharedMemPut(SharedMem_t shMem, void *memBlock);
BaseType_t xSharedMemPutFromISR(SharedMem_t shMem, void *memBlock, BaseType_t *pxHTW);

#endif /* SHAREDMEMORY_H_ */
