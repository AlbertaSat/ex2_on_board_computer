/*
 * csp_transfer.h
 *
 *  Created on: Jun 7, 2018
 *      Author: andre
 */

#ifndef SRC_CSP_TRANSFER_H_
#define SRC_CSP_TRANSFER_H_




#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdint.h>

//\\timeouts and delays
#define CSP_ACCEPT_TIMEOUT 10000
#define CSP_READ_TIMEOUT 1000
#define delay_1000 pdMS_TO_TICKS(1000)


//\\CSP transfer information


typedef enum{
	int8_t toSlave = 1;
	int8_t toMaster = 0;
}transferDirection;

typedef struct {
	uint8_t cspAddress;
	uint8_t i2cAddress;
	uint8_t listenPort;
	char name[10];
}EX2Module;



#endif /* SRC_CSP_TRANSFER_H_ */
