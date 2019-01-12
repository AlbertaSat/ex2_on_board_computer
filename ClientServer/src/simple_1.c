/*
 * send.c
 *
 *  Created on: Jun 5, 2018
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
#include <interfaces/csp_if_lo.h>
#include <csp_interface.h>

#include "em_chip.h"
#include "bsp.h"
#include "bsp_trace.h"

#include <sleep.h>
#include <stdint.h>
#include <stdio.h>
#include <csp/csp.h>
//#include <csp/interfaces/csp_if_kiss.h>
#include <csp/interfaces/csp_if_i2c.h>
#include <csp/arch/csp_thread.h>
////#include <i2c.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <makePrintfWork.h>
#include <csp/csp.h>
#include "em_device.h"
#include "em_chip.h"
/* Using un-exported header file.
 * This is allowed since we are still in libcsp */
#include <csp/arch/csp_thread.h>
#include <FreeRTOS.h>
//#include <interfaces/csp_if_lo.h>
#include <csp_interface.h>

#include "em_chip.h"
#include "bsp.h"
#include "bsp_trace.h"

#include <sleep.h>

#include <i2c_master_slave.h>

#define CSP_ERR -13
#define delay_1000 pdMS_TO_TICKS(1000)
#define CSP_ACCEPT_TIMEOUT 10000
#define CSP_READ_TIMEOUT 1000
#define I2C 9
#define MY_ADDRESS  1			// Address of local CSP node
#define MY_PORT		10
//void csp_create_listener(void){
//	csp_bind()
//}
/*
csp_socket_t csp_listen_sock(void){
//This function will bind to all ports and then listen for something to come through that port.

	csp_socket_t * sock;

	csp_bind(sock, CSP_ANY); //usually bind all ports to the socket and then tdo the switch case for which action to tae, but mayeb tdot that in the calling funciton

	csp_listen(sock, 10);//create 10 connection in the backlog queue.

	return sock;
}*/

int csp_connect_send_(uint8_t i2c_addr, uint8_t CSP_addr, uint8_t * dport, uint8_t sport){//, char * command

	csp_packet_t * packet;
	csp_conn_t * conn;
	//conn = csp_connect(CSP_PRIO_NORM, CSP_addr, 1000, CSP_O_NONE);
	packet = csp_buffer_get(100);
	char command;
	if(packet == NULL){
		// buffer allocation failed
		csp_buffer_free(packet);
		return CSP_ERR_NOMEM;
	}
	if (conn == NULL){
		//connection failed
		csp_buffer_free(packet); // free packet now
		return CSP_ERR;
	}
//#ifdef nothing
	char * msg = command;
	strcpy((char *) packet->data, msg);
packet->length = strlen(msg);
	if (!csp_send(conn, packet, 1000)){
		//sendong the packet failed
		csp_buffer_free(packet); // free packet now
		return CSP_ERR_TX;
	}
	//csp_close(conn);// close the connection!
//#endif
	return CSP_ERR_NONE;
}



#define my_i2c_address 1110000
CSP_DEFINE_TASK(task_server){
	/* Create socket without any socket options */
	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	/* Bind all ports to socket */
	csp_bind(sock, CSP_ANY);

	/* Create 10 connections backlog queue */
	csp_listen(sock, 10);
	csp_conn_t *conn;
	csp_packet_t *packet;
	csp_socket_t *socket;
	csp_packet_t *newpack;
	//socket = csp_listen_sock(); // create the network socket
	while (1) {

			/* Wait for connection, 10000 ms timeout */
			if ((conn = csp_accept(socket, CSP_ACCEPT_TIMEOUT)) == NULL)
				continue;

			/* Read packets. Timout is 100 ms */
			while ((packet = csp_read(conn, 100)) != NULL) {
				switch (csp_conn_dport(conn)) {//what port was the incoming packet *for*?
				case MY_PORT:
					puts("connection on my port, reply with some data.");

					char * message;
					message = "DSfg";
					strcpy((char *)packet->data, message);
					newpack->length = strlen(message);
					csp_send(conn, newpack, 1000);
					/* Process packet here */
					//printf("Packet received on MY_PORT: %s\r\n", packet->data);
					csp_buffer_free(packet);
					break;

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

CSP_DEFINE_TASK(task_client){
	csp_packet_t * packet;
	csp_conn_t * conn;
csp_packet_t * newpack;

	while (1) {

			/**
			 * Try ping
			 */
			const portTickType delay = pdMS_TO_TICKS(1000);
			vTaskDelay(delay);

			//int result = csp_ping(MY_ADDRESS, 100, 100, CSP_O_NONE);
			//printf("Ping result %d [ms]\r\n", result);

			vTaskDelay(delay);
			//csp_packet_t * packet;
				//csp_conn_t * conn;
				conn = csp_connect(CSP_PRIO_NORM, MY_ADDRESS, MY_PORT, 1000, CSP_O_NONE);
				packet = csp_buffer_get(100);

				if(packet == NULL){
					// buffer allocation failed
					csp_buffer_free(packet);
					return CSP_TASK_RETURN;
				}
				if (conn == NULL){
					//connection failed
					csp_buffer_free(packet); // free packet now
					return CSP_TASK_RETURN;
				}
			//#ifdef nothing
				char * msg;
				msg = "Adfg";
				strcpy((char *) packet->data, msg);
			packet->length = strlen(msg);
				if (!csp_send(conn, packet, 1000)){
					//sendong the packet failed
					csp_buffer_free(packet); // free packet now
					return CSP_TASK_RETURN;
				}
				vTaskDelay(delay);
		while((newpack = csp_read(conn, 100))!=NULL){
			printf("%s", newpack->data);
		}

	}
		return CSP_TASK_RETURN;
	}

int main(int argc, char * argv[]) {


	SWO_SetupForPrint();
	CHIP_Init();
#ifdef I2C__
	extern csp_iface_t csp_if_i2c;
	//csp_iflist_add(&csp_if_i2c); - this is in the csp_i2c_init function
	csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_i2c, CSP_NODE_MAC);
	csp_i2c_init((uint8_t) my_i2c_address,(int) I2C, (int) 100);//local I2C address, "device handle??", speed
#endif


	/* Init buffer system with 10 packets of maximum 300 bytes each */


	//puts("Initialising CSP\r\n");
	csp_buffer_init(5, 300);

	/* Init CSP with address MY_ADDRESS */
	csp_init(MY_ADDRESS);

	/* Start router task with 500 word stack, OS task priority 1 */
	csp_route_start_task(500, 1);
	extern csp_iface_t csp_if_lo;
	csp_iflist_add(&csp_if_lo);
	csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_lo, CSP_NODE_MAC);
	/* Enable debug output from CSP */
	if ((argc > 1) && (strcmp(argv[1], "-v") == 0)) {
		puts("Debug enabed\r\n");
		csp_debug_toggle_level(3);
		csp_debug_toggle_level(4);

		printf("Conn table\r\n");
		csp_conn_print_table();

		printf("Route table\r\n");
		csp_route_print_table();

		printf("Interfaces\r\n");
		csp_route_print_interfaces();

	}

	/**
	 * Initialise example threads, using pthreads.
	 */

	/* Server */
	puts("Starting Server task\r\n");
	csp_thread_handle_t handle_server;
	csp_thread_create(task_server, "SERVER", 1000, NULL, 1, &handle_server);

	/* Client */
	puts("Starting Client task\r\n");
	csp_thread_handle_t handle_client;
	csp_thread_create(task_client, "SERVER", 1000, NULL, 1, &handle_client);
	vTaskStartScheduler();
	/* Wait for execution to end (ctrl+c) */
	while(1) {
		//csp_sleep_ms(1000);
	}

	return 0;

}



//come functions to make this code more modular








