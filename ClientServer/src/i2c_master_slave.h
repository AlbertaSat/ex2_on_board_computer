/*
 * i2c_master_slave.h
 *
 *  Created on: Jun 5, 2018
 *      Author: andrew
 */

#ifndef SRC_I2C_MASTER_SLAVE_H_
#define SRC_I2C_MASTER_SLAVE_H_

void setupI2C(void);
void receiveI2CData(void);
void performI2CTransfer(void);

#endif /* SRC_I2C_MASTER_SLAVE_H_ */
