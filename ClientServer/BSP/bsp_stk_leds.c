/***************************************************************************//**
 * @file
 * @brief Board support package API for GPIO leds on STK's.
 * @version 5.4.0
 *******************************************************************************
 * # License
 * <b>Copyright 2016 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "bsp.h"
#if defined(BSP_IO_EXPANDER)
#include "bsp_stk_ioexp.h"
#endif

//****************************************************************************

#if defined(BSP_GPIO_LEDS) || defined(BSP_IO_EXPANDER)
/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if !defined(BSP_IO_EXPANDER)
#define MAX_NUMBER_OF_SUBLEDS   3
#define SUBLEDS_ON              0xffffffff
#define SUBLEDS_OFF             0

typedef struct {
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} tLedIo;

typedef struct {
  bool          polarity;
  uint8_t       subLedCnt;
  tLedIo        io[MAX_NUMBER_OF_SUBLEDS];
} tLedExtIo;
#endif

#if !defined(BSP_IO_EXPANDER)
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
static const tLedExtIo ledExtArray[BSP_NO_OF_LEDS] = BSP_GPIO_EXTLEDARRAY_INIT;
#else
static const tLedIo ledArray[BSP_NO_OF_LEDS] = BSP_GPIO_LEDARRAY_INIT;
#endif
#endif

//****************************************************************************

#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
uint32_t BSP_ExtLedGet(int ledNo)
{
  uint32_t retVal = 0;
  uint32_t mask = 1;
  int subLed;
  bool pinLevel;

  if ((ledNo >= 0) && (ledNo < BSP_NO_OF_LEDS)) {
    for (subLed = 0; subLed < ledExtArray[ledNo].subLedCnt; subLed++) {
      pinLevel = GPIO_PinOutGet(ledExtArray[ledNo].io[subLed].port, ledExtArray[ledNo].io[subLed].pin) != 0;
      retVal += (pinLevel == ledExtArray[ledNo].polarity) ? mask : 0;
      mask <<= 1;
    }
  }
  return retVal;
}

void BSP_ExtLedSet(int ledNo, uint32_t subLeds)
{
  int subLed;
  uint32_t mask = 1;

  if ((ledNo >= 0) && (ledNo < BSP_NO_OF_LEDS)) {
    for (subLed = 0; subLed < ledExtArray[ledNo].subLedCnt; subLed++) {
      if ((bool)((subLeds & mask) == mask) == ledExtArray[ledNo].polarity) {
        GPIO_PinOutSet(ledExtArray[ledNo].io[subLed].port, ledExtArray[ledNo].io[subLed].pin);
      } else {
        GPIO_PinOutClear(ledExtArray[ledNo].io[subLed].port, ledExtArray[ledNo].io[subLed].pin);
      }
      mask <<= 1;
    }
  }
}
#endif

int BSP_LedsInit(void)
{
  /* If an I/O expander is used, assume that BSP_Init(BSP_INIT_IOEXP)
   * and BSP_PeripheralAccess(BSP_IOEXP_LEDS, true) has been called.
   */
#if !defined(BSP_IO_EXPANDER)
  int ledNo;
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
  int subLed;
#endif

  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  for (ledNo = 0; ledNo < BSP_NO_OF_LEDS; ledNo++) {
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
    for (subLed = 0; subLed < ledExtArray[ledNo].subLedCnt; subLed++) {
      GPIO_PinModeSet(ledExtArray[ledNo].io[subLed].port, ledExtArray[ledNo].io[subLed].pin, gpioModePushPull, ledExtArray[ledNo].polarity ? 0 : 1);
    }
#else
    GPIO_PinModeSet(ledArray[ledNo].port, ledArray[ledNo].pin, gpioModePushPull, 0);
#endif
  }
#endif
  return BSP_STATUS_OK;
}

uint32_t BSP_LedsGet(void)
{
#if defined(BSP_IO_EXPANDER)
  uint8_t value;
  uint32_t retVal;

  ioexpReadReg(BSP_IOEXP_REG_LED_CTRL, &value);
  retVal = value & BSP_IOEXP_REG_LED_CTRL_LED_MASK;

#else
  int ledNo;
  uint32_t retVal, mask;
  bool ledOn;

  for (ledNo = 0, retVal = 0, mask = 0x1; ledNo < BSP_NO_OF_LEDS; ledNo++, mask <<= 1 ) {
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
    ledOn = BSP_ExtLedGet(ledNo) != SUBLEDS_OFF;
#else
    ledOn = GPIO_PinOutGet(ledArray[ledNo].port, ledArray[ledNo].pin) != 0;
#endif
    if (ledOn) {
      retVal |= mask;
    }
  }
#endif
  return retVal;
}

int BSP_LedsSet(uint32_t leds)
{
#if defined(BSP_IO_EXPANDER)
  uint8_t value;

  value = (leds & BSP_IOEXP_REG_LED_CTRL_LED_MASK)
          | BSP_IOEXP_REG_LED_CTRL_DIRECT;
  return ioexpWriteReg(BSP_IOEXP_REG_LED_CTRL, value);

#else
  int ledNo;
  uint32_t mask;

  for (ledNo = 0, mask = 0x1; ledNo < BSP_NO_OF_LEDS; ledNo++, mask <<= 1) {
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
    BSP_ExtLedSet(ledNo, ((leds & mask) == mask) ? SUBLEDS_ON : SUBLEDS_OFF);
#else
    if (leds & mask) {
      GPIO_PinOutSet(ledArray[ledNo].port, ledArray[ledNo].pin);
    } else {
      GPIO_PinOutClear(ledArray[ledNo].port, ledArray[ledNo].pin);
    }
#endif
  }
  return BSP_STATUS_OK;
#endif
}

int BSP_LedClear(int ledNo)
{
  if ((ledNo >= 0) && (ledNo < BSP_NO_OF_LEDS)) {
#if defined(BSP_IO_EXPANDER)
    return ioexpRegBitsSet(BSP_IOEXP_REG_LED_CTRL, false, 1 << ledNo);
#else
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
    BSP_ExtLedSet(ledNo, SUBLEDS_OFF);
#else
    GPIO_PinOutClear(ledArray[ledNo].port, ledArray[ledNo].pin);
#endif
    return BSP_STATUS_OK;
#endif
  }
  return BSP_STATUS_ILLEGAL_PARAM;
}

int BSP_LedGet(int ledNo)
{
#if defined(BSP_IO_EXPANDER)
  uint8_t value;
#endif
  int retVal = BSP_STATUS_ILLEGAL_PARAM;

  if ((ledNo >= 0) && (ledNo < BSP_NO_OF_LEDS)) {
#if defined(BSP_IO_EXPANDER)
    ioexpReadReg(BSP_IOEXP_REG_LED_CTRL, &value);
    retVal = (value & BSP_IOEXP_REG_LED_CTRL_LED_MASK) >> ledNo;
#else
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
    retVal = (BSP_ExtLedGet(ledNo) == SUBLEDS_OFF) ? 0 : 1;
#else
    retVal = (int)GPIO_PinOutGet(ledArray[ledNo].port, ledArray[ledNo].pin);
#endif
#endif
  }
  return retVal;
}

int BSP_LedSet(int ledNo)
{
  if ((ledNo >= 0) && (ledNo < BSP_NO_OF_LEDS)) {
#if defined(BSP_IO_EXPANDER)
    return ioexpRegBitsSet(BSP_IOEXP_REG_LED_CTRL, true, (1 << ledNo));
#else
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
    BSP_ExtLedSet(ledNo, SUBLEDS_ON);
#else
    GPIO_PinOutSet(ledArray[ledNo].port, ledArray[ledNo].pin);
#endif
    return BSP_STATUS_OK;
#endif
  }
  return BSP_STATUS_ILLEGAL_PARAM;
}

int BSP_LedToggle(int ledNo)
{
#if defined(BSP_IO_EXPANDER)
  uint8_t value;
#else
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
  uint32_t ledState;
#endif
#endif

  if ((ledNo >= 0) && (ledNo < BSP_NO_OF_LEDS)) {
#if defined(BSP_IO_EXPANDER)
    ioexpReadReg(BSP_IOEXP_REG_LED_CTRL, &value);
    value &= BSP_IOEXP_REG_LED_CTRL_LED_MASK;
    value ^= (1 << ledNo);
    value |= BSP_IOEXP_REG_LED_CTRL_DIRECT;
    return ioexpWriteReg(BSP_IOEXP_REG_LED_CTRL, value);
#else
#if defined(BSP_GPIO_EXTLEDARRAY_INIT)
    ledState = BSP_ExtLedGet(ledNo);
    BSP_ExtLedSet(ledNo, (ledState == SUBLEDS_OFF) ? SUBLEDS_ON : SUBLEDS_OFF);
#else
    GPIO_PinOutToggle(ledArray[ledNo].port, ledArray[ledNo].pin);
#endif
    return BSP_STATUS_OK;
#endif
  }
  return BSP_STATUS_ILLEGAL_PARAM;
}

/** @endcond */
#endif /* BSP_GPIO_LEDS */
