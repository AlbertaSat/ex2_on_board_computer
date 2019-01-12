/*
 * client.c
 *
 *  Created on: Aug 24, 2018
 *      Author: andre
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "makePrintfWork.h"
#include <csp/csp.h>
#include "em_device.h"
#include "em_chip.h"
/* Using un-exported header file.
 * This is allowed since we are still in libcsp */
#include <csp/arch/csp_thread.h>
#include <FreeRTOS.h>
//#include <interfaces/csp_if_lo.h>
#include <interfaces/csp_if_i2c.h>
#include <csp_interface.h>
#include <csp_types.h>
#include "em_chip.h"
#include "bsp.h"
#include "bsp_trace.h"
//#include "em_i2c.h"
#include <sleep.h>
#include "cspI2C_EFM32_M3.h"

#define MY_ADDRESS  1			// Address of local CSP node
#define MY_PORT		10			// Port to send test traffic to


extern csp_iface_t csp_if_i2c;

CSP_DEFINE_TASK(task_server) {

	/* Create socket without any socket options */
	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	/* Bind all ports to socket */
	csp_bind(sock, CSP_ANY);

	/* Create 10 connections backlog queue */
	csp_listen(sock, 10);

	/* Pointer to current connection and packet */
	csp_conn_t *conn;
	csp_packet_t *packet;
	int port;
	/* Process incoming connections */
	while (1) {
		puts("in the while loop");

		/* Wait for connection, 10000 ms timeout */
		if ((conn = csp_accept(sock, 10000)) == NULL)
			continue;

		//xNetworkInterfaceInitialise();

		/* Read packets. Timout is 100 ms */
		while ((packet = csp_read(conn, 100)) != NULL) {
			switch (port = csp_conn_dport(conn)) {
			printf("%d", port);
			case MY_PORT:
				/* Process packet here */
				printf("Packet received on MY_PORT: %s\r\n", packet->data);
				csp_buffer_free(packet);
				break;
			//case CSP_FTP:
				//puts("myabe check for something?");
			default:
				puts("reply to ping");
				/* Let the service handler reply pings, buffer use, etc. */
				csp_service_handler(conn, packet);
				break;
			}
		}

		/* Close current connection, and handle next */
		csp_close(conn);

	}

	return CSP_TASK_RETURN;

}

int main(int argc, char * argv[]) {

	/**
	 * Initialise CSP,
	 * and initialize I2C!
	 */
	SWO_SetupForPrint();
	CHIP_Init();

	csp_init(MY_ADDRESS); // I changed this to make csp initialise with i2c not lo.

	csp_buffer_init(300, 300);
	// TODO Keith's hackery I2C
	cspDMA_Init(CSP_HPROT); // DMA Controller

	csp_i2c_init(MY_ADDRESS, 1, 400); // Init I2C for I2C1 on the EFM32 GG (csp_i2c_init)

	csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_i2c, CSP_NODE_MAC);
	/* Start router task with 500 word stack, OS task priority 1 */
	csp_route_start_task(500, 1);

	/**
	 * Initialise server using the csp api.
	 */

	/* Server */
	puts("Starting Server task\r\n");
	csp_thread_handle_t handle_server;
	csp_thread_create(task_server, "SERVER", 1000, NULL, 0, &handle_server);
	vTaskStartScheduler();

	while(1) {
		//should not end up here!
	}

	return 0;

}
