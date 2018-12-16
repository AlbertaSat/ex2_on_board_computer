/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
Copyright (C) 2012 AAUSAT3 Project (http://aausat3.space.aau.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>

/** Example defines */
#define MY_ADDRESS  1			// Address of local CSP node
#define MY_PORT		10			// Port to send test traffic to

CSP_DEFINE_TASK(task_server) {

	// Create socket without any socket options
	csp_socket_t *sock = csp_socket(CSP_SO_XTEAREQ | CSP_SO_HMACREQ | CSP_SO_CRC32REQ | CSP_SO_CONN_LESS);

	// Bind all ports to socket
	csp_bind(sock, MY_PORT);

	// Pointer to current connection and packet
	csp_packet_t *packet;

	while (1) {
		//receive packets
		packet = csp_recvfrom(sock, 1000);
		
		if (packet != NULL) {	
			char buff[100];
			memccpy(buff, packet->data, 1, packet->length);
			printf("%s\n", buff);
			csp_buffer_free(packet);;

		}		
	}

	return CSP_TASK_RETURN;

}

CSP_DEFINE_TASK(task_client) {

	csp_packet_t * packet;

	while (1) {

		csp_sleep_ms(1000);
		unsigned int opts = CSP_SO_XTEAREQ | CSP_SO_HMACREQ | CSP_SO_CRC32REQ | CSP_SO_CONN_LESS;


		// Get packet buffer for data 
		packet = csp_buffer_get(100);

		if (packet == NULL) {
			// Could not get buffer element
			printf("Failed to get buffer element\n");
			return CSP_TASK_RETURN;
		}

		char *msg = "Hello World";
		strncpy((char *) packet->data, msg, 30);
	
		// Set packet length 
		packet->length = strlen(msg);
		csp_sendto(0, MY_ADDRESS, MY_PORT, 0, opts, packet, 1000);

		// TODO learn what this does
		// csp_sendto_reply
	
	}
	return CSP_TASK_RETURN;
}

int main(int argc, char * argv[]) {

	/**
	 * Initialise CSP,
	 * No physical interfaces are initialised in this example,
	 * so only the loopback interface is registered.
	 */

	/* Init buffer system with 10 packets of maximum 300 bytes each */
	printf("Initialising CSP\r\n");
	csp_buffer_init(10, 300);

	/* Init CSP with address MY_ADDRESS */
	csp_init(MY_ADDRESS);

	/* Start router task with 500 word stack, OS task priority 1 */
	csp_route_start_task(1000, 1);

	/* Enable debug output from CSP */
	if ((argc > 1) && (strcmp(argv[1], "-v") == 0)) {
		printf("Debug enabed\r\n");
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
	printf("Starting Server task\r\n");
	csp_thread_handle_t handle_server;
	csp_thread_create(task_server, "SERVER", 1000, NULL, 0, &handle_server);

	/* Client */
	printf("Starting Client task\r\n");
	csp_thread_handle_t handle_client;
	csp_thread_create(task_client, "CLIENT", 1000, NULL, 0, &handle_client);

	/* Wait for execution to end (ctrl+c) */
	while(1) {
		csp_sleep_ms(100000);
	}
	return 0;

}