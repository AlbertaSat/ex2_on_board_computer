/*
 * rtcmk.c
 *
 *  Created on: May 28, 2018
 *  Most recent edit: Aug 11, 2020
 *      Author: sdamk, jdlazaru
 *
 *
 *
 *      TODO: Check if interrupts are needed/how to implement
 */

#include "HL_i2c.h"
#include <rtcmk.h>

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Set content of a register.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit.
 *
 * @param[in] reg
 *   Register to write (input register cannot be written).
 *
 * @param[in] val
 *   Value used when writing to register.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
int RTCMK_RegisterSet(/* I2C_TypeDef *i2c, */
                         uint8_t addr,
                         RTCMK_Register_TypeDef reg,
                         uint8_t val)
{

  uint8_t data;

  data = val;

  i2cSetDirection(i2cREG1, I2C_TRANSMITTER);
  i2cSetCount(i2cREG1, 1);
  i2cSetMode(i2cREG1, I2C_MASTER);
  i2cSetStop(i2cREG1);
  i2cSetStart(i2cREG1);
  i2cSend(i2cREG1, 1, &data);

  /* Wait until Bus Busy is cleared */
  while(i2cIsBusBusy(i2cREG1) == true);

  /* Wait until Stop is detected */
  while(i2cIsStopDetected(i2cREG1) == 0);

  /* Clear the Stop condition */
  i2cClearSCD(i2cREG1);

//  unsigned int retries = 0;

//  while (I2C_getStatus(i2c) == i2cTransferInProgress)
//  {
//    /* Enter EM1 while waiting for I2C interrupt */
//    if(MAX_RETRIES < retries++)break;
//    /* Enter EM1 while waiting for I2C interrupt */
//    EMU_EnterEM1();
//    /* Could do a timeout function here. */
//  }
  
  return((int)i2cIsBusBusy(i2cREG1));
}

/***************************************************************************//**
 * @brief
 *   Get current content of a register.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit.
 *
 * @param[in] reg
 *   Register to read.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if register read, <0 if unable to read register.
 ******************************************************************************/
int RTCMK_RegisterGet(/*I2C_TypeDef *i2c,*/
                         uint8_t addr,
                         RTCMK_Register_TypeDef reg,
                         uint8_t *val)
{
  /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2cREG1, addr);

    i2cSetDirection(i2cREG1, I2C_TRANSMITTER);
    i2cSetCount(i2cREG1, 1);
    i2cSetMode(i2cREG1, I2C_MASTER);
    i2cSetStop(i2cREG1);
    i2cSetStart(i2cREG1);
    i2cSend(i2cREG1, 1, &reg);

    while(i2cIsBusBusy(i2cREG1) == true);
    while(i2cIsStopDetected(i2cREG1) == 0);
    i2cClearSCD(i2cREG1);

    int temp;
    for (temp = 0; temp < 0x10000; temp++);//temporary fix... don't want delay down the road

    i2cSetSlaveAdd(i2cREG1, addr);
    /* Set direction to receiver */
    i2cSetDirection(i2cREG1, I2C_RECEIVER);
    i2cSetCount(i2cREG1, 1);
    /* Set mode as Master */
    i2cSetMode(i2cREG1, I2C_MASTER);
    i2cSetStop(i2cREG1);
    /* Transmit Start Condition */
    i2cSetStart(i2cREG1);
    *val = i2cReceiveByte(i2cREG1);

//  unsigned int retries = 0;
//  while (I2C_getStatus(i2c) == i2cTransferInProgress)
//  {
//    /* Enter EM1 while waiting for I2C interrupt */
//    if(MAX_RETRIES < retries++)break;
//    /* Enter EM1 while waiting for I2C interrupt */
//    EMU_EnterEM1();
//    /* Could do a timeout function here. */
//  }

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2cREG1) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2cREG1) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2cREG1);

  return((int)i2cIsBusBusy(i2cREG1));
}

/***************************************************************************//**
 * @brief
 *   Write 0's to time and calender registors (0x00 to 0x06).
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit.
 *
 * @return
 *   Returns 0 if registers written, <0 if unable to write to registers.
 ******************************************************************************/
int RTCMK_ResetTime(/*I2C_TypeDef *i2c,*/
                         uint8_t addr)
{

  uint8_t data[8] = {0};

  data[0] = ((uint8_t)RTCMK_RegSec) << 1;

  i2cSetDirection(i2cREG1, I2C_TRANSMITTER);
  i2cSetCount(i2cREG1, 8);
  i2cSetMode(i2cREG1, I2C_MASTER);
  i2cSetStop(i2cREG1);
  i2cSetStart(i2cREG1);
  i2cSend(i2cREG1, 8, data);

  /* Wait until Bus Busy is cleared */
  while(i2cIsBusBusy(i2cREG1) == true);

  /* Wait until Stop is detected */
  while(i2cIsStopDetected(i2cREG1) == 0);

  /* Clear the Stop condition */
  i2cClearSCD(i2cREG1);

//  while (I2C_getStatus(i2c) == i2cTransferInProgress)
//  {
//    /* Enter EM1 while waiting for I2C interrupt */
//    EMU_EnterEM1();
//    /* Could do a timeout function here. */
//  }

  return((int)i2cIsBusBusy(i2cREG1));
}

/***************************************************************************//**
 * @brief
 *   Returns current content of seconds register in decimal.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit. 
 *
 * @param[out] val
 *   Reference to place result.
 *
 * @return
 *   Returns 0 if registers written, <0 if unable to write to registers.
 ******************************************************************************/
int RTCMK_ReadSeconds(/*I2C_TypeDef *i2c,*/
                       	uint8_t addr,
                       	uint8_t *val)
{
  int ret = -1;

  uint8_t tmp = 0;

  ret = RTCMK_RegisterGet(addr,RTCMK_RegSec,&tmp);
  if (ret < 0)
  {
    return(ret);
  }

  tmp &= _RTCMK_SEC_SEC_MASK;

  *val = ((tmp & 0xF0) >> 4) * 10 + (tmp & 0x0F);

  return(ret);
}

/***************************************************************************//**
 * @brief
 *   Returns current content of minutes register in decimal.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit. 
 *
 * @param[out] val
 *   Reference to place result.
 *
 * @return
 *   Returns 0 if registers written, <0 if unable to write to registers.
 ******************************************************************************/
int RTCMK_ReadMinutes(/*I2C_TypeDef *i2c,*/
                       	uint8_t addr,
                       	uint8_t *val)
{
  int ret = -1;

  uint8_t tmp = 0;

  ret = RTCMK_RegisterGet(addr,RTCMK_RegMin,&tmp);
  if (ret < 0)
  {
    return(ret);
  }

  tmp &= _RTCMK_MIN_MIN_MASK;

  *val = ((tmp & 0xF0) >> 4) * 10 + (tmp & 0x0F);

  return(ret);
}

/***************************************************************************//**
 * @brief
 *   Returns current content of hours register in decimal.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit. 
 *
 * @param[out] val
 *   Reference to place result.
 *
 * @return
 *   Returns 0 if registers written, <0 if unable to write to registers.
 ******************************************************************************/
int RTCMK_ReadHours(/*I2C_TypeDef *i2c,*/
                       	uint8_t addr,
                       	uint8_t *val)
{
  int ret = -1;

  uint8_t tmp = 0;

  ret = RTCMK_RegisterGet(addr,RTCMK_RegHour,&tmp);
  if (ret < 0)
  {
    return(ret);
  }

  tmp &= _RTCMK_HOUR_HOUR_MASK;

  *val = ((tmp & 0xF0) >> 4) * 10 + (tmp & 0x0F);

  return(ret);
}
