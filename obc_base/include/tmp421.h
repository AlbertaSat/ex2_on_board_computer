// SPDX-License-Identifier: GPL-2.0-or-later
/* tmp421.c
 *
 * Copyright (C) 2009 Andre Prendel <andre.prendel@gmx.de>
 * Preliminary support by:
 * Melvin Rook, Raymond Ng
 */

/*
 * Driver for the Texas Instruments TMP421 SMBus temperature sensor IC.
 * Supported models: TMP421, TMP422, TMP423, TMP441, TMP442
 */


//CODE TO MAYBE BE ADDED TO I2C DRIVERS BELOW
void i2cSlaveWriteReg(uint8 sadd, uint8 reg, uint8 data);

uint8 i2cSlaveRead1ByteReg(uint8 sadd, uint8 reg);
uint16 i2cSlaveRead2ByteReg(uint8 sadd, uint8 reg);

int temp_from_sint16(sint16 reg);

int temp_from_uint16(uint16 reg);

struct tmp421_data *tmp421_update_device(uint8 sadd);

int tmp421_read(uint8 sadd, int channel, long *val);



int tmp421_init_client(uint8 sadd);

int tmp421_detect(uint8 sadd);


void TMP421test(uint8 sadd);


