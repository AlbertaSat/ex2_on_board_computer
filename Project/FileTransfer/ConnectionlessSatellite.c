#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>

/** Example defines */
#define GROUND_STATION_ADDRESS  0			// Address of local CSP node
#define GROUND_STATION_PORT		10			// Port to send test traffic to
#define MY_ADDRESS 1

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
		csp_sendto(0, GROUND_STATION_ADDRESS, GROUND_STATION_PORT, 0, opts, packet, 1000);

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