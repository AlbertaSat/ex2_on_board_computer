/*
 * board_io_tests.h
 *
 *  Created on Aug 13, 2020
 *
 *      Author: joshdellaz
 */

#include "HL_het.h"
#include "HL_gio.h"
#include "HL_i2c.h"
#include "HL_sci.h"
#include "HL_can.h"
#include "HL_spi.h"

void InitIO(void);
void UARTTxTest(sciBASE_t * regset);
void UARTRxTest(sciBASE_t * regset);
void SPIMasterTxTest(spiBASE_t * regset);
void SPIMasterRxTest(spiBASE_t * regset);
void I2CMasterTxTest(i2cBASE_t * regset, uint8_t addr);
void I2CMasterRxTest(i2cBASE_t * regset, uint8_t addr);
void I2CSlaveRxTest(i2cBASE_t * regset, uint8_t addr);
void I2CSlaveTxTest(i2cBASE_t * regset, uint8_t addr);
void CANTxTest(canBASE_t * regset, uint8_t msgbox);
void CANRxTest(canBASE_t * regset, uint8_t msgbox);
