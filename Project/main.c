

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

//File System includes
#include <redfs.h>
#include <redposix.h>
#include <redfse.h>
#include <redconf.h>
#include <redvolume.h>
#include <redtests.h>

//Custom FileSystem tasks
#include "FileSystemTasks.h"

// Arguments to mount redfs filesystems
struct redfs_args
{
	const char *fspec;  // Name of device to mount
	unsigned int flags; // REDFSMNT_* flags
};

#define REDFSMNT_FMTONCE 0x1U   // format if volume cannot be mounted
#define REDFSMNT_FMTALWAYS 0x2U // format always

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

	int32_t iErr;
	const char *pszVolume0 = gaRedVolConf[0].pszPathPrefix;
	const char *pszVolume1 = gaRedVolConf[1].pszPathPrefix;

	iErr = red_init();
	if (iErr == -1)
	{
		fprintf(stderr, "Unexpected error %d from red_init()\n", (int)red_errno);
		exit(red_errno);
	}

	iErr = red_format(pszVolume0);
	if (iErr == -1)
	{
		fprintf(stderr, "Unexpected error %d from red_format()\n", (int)red_errno);
		exit(red_errno);
	}

	iErr = red_mount(pszVolume0);
	if (iErr == -1)
	{
		fprintf(stderr, "Unexpected error %d from red_mount()\n", (int)red_errno);
		exit(red_errno);
	}

	iErr = red_format(pszVolume1);
	if (iErr == -1)
	{
		fprintf(stderr, "Unexpected error %d from red_format()\n", (int)red_errno);
		exit(red_errno);
	}

	iErr = red_mount(pszVolume1);
	if (iErr == -1)
	{
		fprintf(stderr, "Unexpected error %d from red_mount()\n", (int)red_errno);
		exit(red_errno);
	}

	//FileSystem Tasks
	vStartFSWriteTask(mainCREATOR_TASK_PRIORITY);


	
	/* Start the scheduler itself. */
	vTaskStartScheduler();

	return 0;
}