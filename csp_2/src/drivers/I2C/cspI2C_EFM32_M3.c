/*****************************************************************************
 * cspI2C_EFM32_M3.h
 *
 *  Created on: Jun 29th, 2018
 *  Author: kgmills
 *
 *  Please see the header of the paired cspI2C_EFM32_M3.h file for details
 *
 ******************************************************************************/

#include "cspI2C_EFM32_M3.h"

/***********
 * GLOBALS
 **********/
// Error Flag for Tx
static volatile uint16_t transmissionError;

// Pointer to structure where the I2C Registers are found. Assigned at runtime depending on module
static I2C_TypeDef *I2CRegs;

// Rx Buffer - Constantly changes but needed for ISR.
static uint8_t *i2c_Rx;

// Rx buffer index/first transmission flag.
static volatile bool i2c_RxInProgress, firstRx;

// FreeRTOS handles
static SemaphoreHandle_t busySem, waitSem; // Tx semaphores
static QueueHandle_t 	 rxIndexQueue; // Rx Queues

/********************************************************************
 * @brief Inline function for calculating the address of the
 * CTRL register of the RX Channel's Descriptor
 */
static inline int16_t *getRxDMACtrlAddr() {
	return (int16_t*)(DMA->CTRLBASE + (DMA_CHANNEL_I2C_RX * CHANNEL_MULT_OFFSET) + CTRL_ADD_OFFSET);
}

/********************************************************************
 * @brief Function called when DMA transfer is complete.
 *
 * THIS FUNCTION IS CALLED IN AN ISR CONTEXT BY THE DMA'S IRQ
 *
 * For details on how to recover the number of bytes that has been received,
 * please visit the Silicon Labs Reference Manual for the
 * EFM32 Giant Gecko with the ARM Cortex-M3 processor.
 * https://www.silabs.com/documents/public/reference-manuals/EFM32GG-RM.pdf
 *
 * Chapter 8 - DMA; Pages 64-66 on the n_minus_1 data field
 *******************************************************************/
void i2cTransferComplete(unsigned int channel, bool primary, void *user) {

	/* Ignore unused parameters */
	(void) primary;
	(void) user;

	// I2C Transmission Channel, and the DMA has dumped all data to the TXDATA
	// buffer, then allow the I2C module to send a STOP once it is out of data.
	// This setting is cleared in the MSTOP condition of the I2C IRQ.
	if (channel == DMA_CHANNEL_I2C_TX) {
		I2CRegs->CTRL |= I2C_CTRL_AUTOSE;
	}

	// RX has completed
	// Calculate the number of bytes that were received in a transmission
	// And send that number to the receiver task for processing to the next layer.
	// The I2C IRQ will send the data buffer.
	else if (channel == DMA_CHANNEL_I2C_RX) {

		/* VERY IMPORTANT THIS IS HOW YOU GET RX DATA SIZE!!!" */
		uint16_t count = I2C_MTU - ((*getRxDMACtrlAddr() & TRANS_REMAIN_MASK) >> TRANS_REMAIN_SHIFT) -1 ;

		// I literally put this here to prevent a size misalignment on the first transfer.
		if (firstRx) {
			firstRx = false;
			count--;
		}

		if (xQueueSendFromISR(rxIndexQueue, &count, NULL) != pdTRUE) {
			transmissionError |= F_QUEUE_ERR;
		}
	}
}

/************************************************************
 * @brief Function CSP calls to send.
 ***********************************************************/
int i2c_send(int handle, i2c_frame_t *frame, uint16_t timeout) {

	// Wait for the current frame the driver is trying to send to be sent successfully
	// or run out of re-tries.
	if (xSemaphoreTake(waitSem, timeout) != pdTRUE) {
		return CSP_ERR_TIMEDOUT;
	}

	for (uint8_t i = 0; i < frame->retries; i++) {

		transmissionError = NO_TRANS_ERR; // Set error flag to zero. This can be modified by the ISR
		I2CRegs->CMD |= I2C_CMD_CLEARTX;  // Clear the Tx Buffer and Tx Shift Register

		I2CRegs->TXDATA = frame->dest & I2C_WRITE; // Load slave address.

		// Prime/invoke the DMA for the transfer. It will load a new byte into
		// the Tx register when the Tx ISR flag goes up.
		// I assume the header is not bad.
		DMA_ActivateBasic(DMA_CHANNEL_I2C_TX,
				true,
				false,
				&(I2CRegs->TXDATA),
				frame,
				CSP_I2C_HEADER_LEN + frame->len - 1);

		I2CRegs->CMD |= I2C_CMD_START; // Issue the start condition

		// Pend the semaphore.
		if (xSemaphoreTake(busySem, timeout) != pdTRUE) {
			transmissionError |= TIMEOUT_ERR;
		}

		// A driver-layer error has occurred. It may have been a timeout, or arbitration was lost, etc
		if (transmissionError != NO_TRANS_ERR) {
			// Right now in all cases, an abort command is issued, and we wait.
			I2CRegs->CMD |= I2C_CMD_ABORT;
			vTaskDelay(portTICK_PERIOD_MS);
		}
		else { // No error occured. Release the "module in use" semaphore and return no error code.
			xSemaphoreGive(waitSem);
			return CSP_ERR_NONE;
		}
	}
	// If we reached this point, we ran out of attempts to send the frame.
	// Driver layer errors occurred. Release the semaphore and report that.
	xSemaphoreGive(waitSem);
	return CSP_ERR_DRIVER;
}

/******************************************************************************
 * @brief Setup the DMA channels for I2C
 * All functions called here are void, so we only return void.
 *****************************************************************************/
static void i2cDMA_ChannelInit(int TX, int RX) {

	/* Initialization Struct, and the Tx Structs */
	DMA_CfgChannel_TypeDef  txChannelConfig;
	DMA_CfgDescr_TypeDef	txDescriptorConfig;

	DMA_CfgChannel_TypeDef  rxChannelConfig;
	DMA_CfgDescr_TypeDef	rxDescriptorConfig;

	/* Setup call-back function */
	dmaCB.cbFunc  = i2cTransferComplete;
	dmaCB.userPtr = NULL;

	/* Setting up TX channel */
	txChannelConfig.highPri   = true;
	txChannelConfig.enableInt = true;
	txChannelConfig.select    = DMAREQ_I2C1_TXBL;
	txChannelConfig.cb        = &dmaCB;
	DMA_CfgChannel(DMA_CHANNEL_I2C_TX, &txChannelConfig);

	/* Setting up TX channel descriptor */
	txDescriptorConfig.dstInc  = dmaDataIncNone;
	txDescriptorConfig.srcInc  = dmaDataInc1;
	txDescriptorConfig.size    = dmaDataSize1;
	txDescriptorConfig.arbRate = dmaArbitrate1;
	txDescriptorConfig.hprot   = 0;
	DMA_CfgDescr(DMA_CHANNEL_I2C_TX, true, &txDescriptorConfig);

	/* Setting up RX channel */
	rxChannelConfig.highPri    = true;
	rxChannelConfig.enableInt  = true;
	rxChannelConfig.select     = DMAREQ_I2C1_RXDATAV;
	rxChannelConfig.cb 		   = &dmaCB;
	DMA_CfgChannel(DMA_CHANNEL_I2C_RX, &rxChannelConfig);

	/* Setting up RX channel descriptor */
	rxDescriptorConfig.dstInc  = dmaDataInc1;
	rxDescriptorConfig.srcInc  = dmaDataIncNone;
	rxDescriptorConfig.size    = dmaDataSize1;
	rxDescriptorConfig.arbRate = dmaArbitrate1;
	rxDescriptorConfig.hprot   = 0;
	DMA_CfgDescr(DMA_CHANNEL_I2C_RX, true, &rxDescriptorConfig);
}

/*****************************************************************************
 * @brief Deletes FreeRTOS objects
 ****************************************************************************/
static inline void i2c_FreeRTOS_Structs_Del() {
	vSemaphoreDelete(busySem);
	vSemaphoreDelete(waitSem);
	vQueueDelete(rxIndexQueue);
}

/******************************************************************************
 * @brief Initializes the Tx Semaphore, Rx Data and Index queues, and shared memory
 *****************************************************************************/
static int8_t i2c_FreeRTOS_Structs_Init() {

	// Create the Tx timeout semaphore.
	busySem = xSemaphoreCreateBinary();
	waitSem = xSemaphoreCreateBinary();
	rxIndexQueue = xQueueCreate(NUM_SH_MEM_BUFS, sizeof(int16_t));

	// If the initialization of one failed, the driver cannot work. Delete all of them!
	if (busySem == NULL || waitSem == NULL || xSemaphoreGive(waitSem) != pdTRUE || \
			rxIndexQueue == NULL) {
		i2c_FreeRTOS_Structs_Del();
		return CSP_ERR_NOMEM;
	}
	return CSP_ERR_NONE; // No error
}

/**************************************************************************//**
 * @brief  Main function
 * Main is called from __iar_program_start, see assembly startup file
 *****************************************************************************/
int i2c_init(uint8_t opt_addr, int handle, int speed) {

	// Initialize all FreeRTOS structures required for the driver to run.
	int rtosErr = i2c_FreeRTOS_Structs_Init();
	// If initalization failed for WHATEVER reason, e.g. this is extensibe code for mutliple
	// bad error conditions, return said error condition.
	if (rtosErr != CSP_ERR_NONE) { return rtosErr;}

	// Decision block for which I2C module to use.
	// First we decide which registers
	// Then enable the clock for the appropriate line.
	// Then decide which IRQ to enable TODO hard coded for 1 on EFM32 with M3
	// Then enable pins
	// Then init DMA channels
	enum IRQn I2C_IRQ;
	if (handle == 0){
		I2CRegs = I2C0;
		CMU_ClockEnable(cmuClock_I2C0, true);
		I2C_IRQ = I2C0_IRQn;

		GPIO_PinModeSet(I2C0_Ports, I2C0_SDA, gpioModeWiredAndPullUpFilter, 1);
		GPIO_PinModeSet(I2C0_Ports, I2C0_SCL, gpioModeWiredAndPullUpFilter, 1);

		i2cDMA_ChannelInit(DMAREQ_I2C0_TXBL, DMAREQ_I2C0_RXDATAV);
	}

	else if (handle == 1) {
		I2CRegs = I2C1;
		CMU_ClockEnable(cmuClock_I2C1, true);
		I2C_IRQ = I2C1_IRQn;

		GPIO_PinModeSet(I2C1_Ports, I2C1_SDA, gpioModeWiredAndPullUpFilter, 1);
		GPIO_PinModeSet(I2C1_Ports, I2C1_SCL, gpioModeWiredAndPullUpFilter, 1);

		i2cDMA_ChannelInit(DMAREQ_I2C1_TXBL, DMAREQ_I2C1_RXDATAV);
	}

	else {
		i2c_FreeRTOS_Structs_Del();
		return CSP_ERR_INVAL;
	}

	/*
	* Changing the priority of I2C1 IRQ.
	* It must be numerically equal to or greater than configMAX_SYSCALL_INTERRUPT_PRIORITY
	* defined in FreeRTOSConfig.h
	* Currently, that is set to 5.
	* The I2C priority level goes to 6, and the DMA 5.
	*/
	NVIC_SetPriority(I2C_IRQ, I2C_INT_PRIO_LEVEL);

	if (speed == STANDARD_CLK) {
		I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
		I2C_Init(I2CRegs, &i2cInit);
	}

	else if (speed == FAST_CLK) {
		I2C_Init_TypeDef i2cInit = { \
			true, \
			true, \
			0, \
			I2C_FREQ_FAST_MAX, \
			i2cClockHLRAsymetric \
		};
		I2C_Init(I2CRegs, &i2cInit);
	}

	else {
		i2c_FreeRTOS_Structs_Del();
		return CSP_ERR_INVAL;
	}

	/* Enable pins at location 1 */
	I2CRegs->ROUTE = I2C_ROUTE_SDAPEN |
			I2C_ROUTE_SCLPEN |
			(I2C_ROUTE_LOC << _I2C_ROUTE_LOCATION_SHIFT);

	/* Setting up to enable slave mode */
	I2CRegs->SADDR = opt_addr;
	I2CRegs->CTRL |= csp_I2C_ctrl;
	I2CRegs->SADDRMASK |= CSP_SADDR_MASK;

	// Set Rx index to zero.
	i2c_RxInProgress = false;

	// We do an operation on the first Rx.
	firstRx = true;

	// Enable interrupts
	I2C_IntClear(I2CRegs, i2c_IFC_flags);
	I2C_IntEnable(I2CRegs, i2c_IEN_flags);
	NVIC_EnableIRQ(I2C_IRQ);

	// We're starting/restarting the board, so it assume the bus is busy
	// We need to either have a clock-high (BITO) timeout, or issue an abort
	if (I2CRegs->STATE & I2C_STATE_BUSY) {
		I2CRegs->CMD = I2C_CMD_ABORT;
	}

	return CSP_ERR_NONE; // Everything went fine. Good to go!
}

/*****************************************************************************
 * @brief I2C Interrupt Handler.
 *        The interrupt table is in assembly startup file startup_efm32.s
 *****************************************************************************/
void I2C1_IRQHandler() {
   
  int flags = I2CRegs->IF;

  /*
   * Conditions that may, but are not guaranteed to, cause a BUSHOLD condition
   * Usually normal operating conditions but take too long
   * Tx/Rx transfer stuff
   */
  if (flags & (I2C_IF_BUSHOLD | I2C_IF_NACK | I2C_IF_ADDR)) {

	  /*
	   * Slave Receiver:
	   * Start Condition on the line has been detected
	   * Automatic Address Matching has determined a Master is trying to talk to this device
	   */
	  if (flags & I2C_IF_ADDR ) {

		  // Ack the address or else it will be interpreted as a NACK.
		  I2CRegs->CMD = I2C_CMD_ACK;

		  i2c_Rx = csp_buffer_get(I2C_MTU + 10); //pSharedMemGetFromISR(i2cSharedMem, NULL);

		  // The buffer pend returned true, so we can accept the transmission.
		  // Get the first byte, set up for AUTO-ACKs so the DMA handles all incoming
		  // And activate the DMA. Done.
		  if (i2c_Rx != NULL) {
			  // Setup DMA Transfer
			  i2c_Rx[0] = I2CRegs->RXDATA;
			  I2CRegs->CTRL |= I2C_CTRL_AUTOACK;
			  i2c_RxInProgress = true;
			  DMA_ActivateBasic(DMA_CHANNEL_I2C_RX,
					  true,
					  false,
					  (void*)i2c_Rx + 1,
					  (void*)&(I2CRegs->RXDATA),
					  I2C_MTU - 1);
		  }

		  // No pointers are availible so we cannot accept the data.
		  // Send a NACK and abort the transmission.
		  else {
			  I2CRegs->CMD = I2C_CMD_NACK | I2C_CMD_ABORT;
			  transmissionError |= E_QUEUE_ERR;
		  }

		  // In all cases the Interrupt flag must be cleared.
		  I2C_IntClear(I2CRegs, I2C_IFC_ADDR | I2C_IFC_BUSHOLD);
	  }

	  /*
	   * Master Transmitter:
	   * NACK Received.
	   * I2C_CTRL_AUTOSN flags ensures we cut transmission on a NACK but we need to report the error.
	   */
	  if (flags & I2C_IF_NACK) {
		  transmissionError |= NACK_ERR;
		  I2C_IntClear(I2CRegs, I2C_IFC_NACK);
		  I2CRegs->CMD |= I2C_CMD_ABORT;
		  xSemaphoreGiveFromISR(busySem, NULL);
	  }

	  /*
	   * Sometimes we encounter BUSHOLD
	   * Most of the time it was because the DMA IRQ was not fast enough
	   * to tell the I2C module to sent it's stop condition when TXBL and TXC are fully
	   * emptied so the bus locks up.
	   */
	  if (I2CRegs->IF & I2C_IF_BUSHOLD) {

		  // Data Transmitted and ACK received. DMA was not fast enough to trigger stop.
		  if (I2CRegs->STATE & I2C_STATE_MASTER) {
			  I2CRegs->CMD |= I2C_CMD_STOP;
		  }

		  else {
			  I2CRegs->CMD |= I2C_CMD_ABORT;
			  DMA->CHENS &= ~DMA_ENABLE_I2C_TX;
			  transmissionError |= ABORT_BUSHOLD;
			  xSemaphoreGiveFromISR(busySem, NULL);
		  }
	  }
  }

  /*
   * Master has transmitted stop condition
   * Transmission is officially over
   * Report success to upper layer
   * Release semaphore
   */
  if (flags & I2C_IF_MSTOP) {
	  I2C_IntClear(I2CRegs, i2c_IFC_flags);
	  flags &= ~I2C_IF_SSTOP;
	  I2CRegs->CTRL &= ~I2C_CTRL_AUTOSE;
	  xSemaphoreGiveFromISR(busySem, NULL);
  }

  /*
   * Stop Condition (or Repeated Start) detected
   * This flag is raised regardless
   * of whether or not the Master was speaking to us
   * CHECK and see if we were spoken to
   * By looking at a boolean set when the I2C_IF_ADDR flag was raised.
   */
  if (flags & (I2C_IF_SSTOP | I2C_IF_RSTART)) {
	  if (i2c_RxInProgress) {

		  //Cast the Rx buffer as a CSP I2C Frame.
		  i2c_frame_t *cspBuf = (i2c_frame_t*)i2c_Rx;

		  // Tell the DMA to stop receiving bytes and report how many bytes were RX'd
		  // NOTE: YOU ARE DEPENDANT ON THE DMA IRQ BEING OF HIGHER PRIORITY THAN THE I2C IRQ
		  // THUS FORCING A CONTEXT SWITCH TO THE DMA IRQ THE MOMENT THIS FLAG IS RAISED!
		  DMA->IFS = DMA_COMPLETE_I2C_RX;
		  if (xQueueReceiveFromISR(rxIndexQueue, &(cspBuf->len), NULL) != pdTRUE) {
		  }

		  i2c_RxInProgress = false;

		  csp_i2c_rx(cspBuf, NULL);
	  }
	  I2CRegs->CTRL &= ~I2C_CTRL_AUTOACK;
      I2C_IntClear(I2CRegs, I2C_IFC_SSTOP | I2C_IFC_RSTART);
  }

  /*
   * All Potential Error Codes/Timeouts
   * Put ARBLOST, BITO, BUSERR, CLTO, here.
   * Raise error flag for Tx task.
   * Cancel Auto-acks, abort transmission, return from ISR.
   * */
  if (flags & (I2C_IF_ARBLOST | I2C_IF_BUSERR | I2C_IF_CLTO | I2C_IF_BITO)) {
	  if (flags & I2C_IF_BITO) {
		  transmissionError |= BITO_ERR;
	  }
	  if (flags & I2C_IF_ARBLOST) {
		  transmissionError |= ARBLOST_ERR;
	  }
	  if (flags & I2C_IF_BUSERR) {
		  transmissionError |= BUSERR_ERR;
	  }
	  if (flags & I2C_IF_CLTO) {
		  transmissionError |= CLTO_ERR;
	  }

	  // Tell the DMA to stop.
	  // Receive it's item but do nothing with it.
	  if (i2c_RxInProgress) {
		  DMA->IFS = DMA_COMPLETE_I2C_RX;
		  uint16_t index;
		  if (xQueueReceiveFromISR(rxIndexQueue, &index, NULL) != pdTRUE) {
		  }
	  }

	  else {
		  DMA->IFS = DMA_COMPLETE_I2C_TX;
	  }

	  I2CRegs->CTRL &= ~I2C_CTRL_AUTOACK;
	  I2C_IntClear(I2CRegs, I2C_IFC_ARBLOST | I2C_IFC_BUSERR | I2C_IFC_CLTO | I2C_IFC_BITO);
	  I2CRegs->CMD = I2C_CMD_ABORT;
	  i2c_RxInProgress = false;
	  csp_buffer_free_isr(i2c_Rx);
	  //xSharedMemPutFromISR(i2cSharedMem, i2c_Rx, NULL);
	  xSemaphoreGiveFromISR(busySem, NULL);
  }
}
