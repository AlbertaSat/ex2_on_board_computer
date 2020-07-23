

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

//Ftp tasks/functions
#include "file_delivery_app.h"
#include "requests.h"

/* Priorities at which the tasks are created. */
#define mainCHECK_TASK_PRIORITY (configMAX_PRIORITIES - 2)
#define mainQUEUE_POLL_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainSEM_TEST_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainBLOCK_Q_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainCREATOR_TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define mainINTEGER_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainGEN_QUEUE_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainFLOP_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainQUEUE_OVERWRITE_PRIORITY (tskIDLE_PRIORITY)

#define mainTIMER_TEST_PERIOD (50)

void vAssertCalled(
unsigned long ulLine, const char * const pcFileName 
)
{
		printf("error line: %lu in file: %s", ulLine, pcFileName);
}



int main(void)
{
	
	/* Start the scheduler itself. */
	vTaskStartScheduler();

	return 0;
}
