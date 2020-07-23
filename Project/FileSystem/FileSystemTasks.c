

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "FileSystemTasks.h"

#include <redfs.h>
#include <redposix.h>
#include <redfse.h>
#include <redconf.h>
#include <redvolume.h>
#include <redtests.h>

//File System includes
/*
#include <redfs.h>
#include <redposix.h>
#include <redfse.h>
#include <redconf.h>
#include <redvolume.h>
#include <redtests.h>

//Custom FileSystem tasks
#include "FileSystemTasks.h"
*/


// Arguments to mount redfs filesystems
struct redfs_args
{
	const char *fspec;  // Name of device to mount
	unsigned int flags; // REDFSMNT_* flags
};

#define REDFSMNT_FMTONCE 0x1U   // format if volume cannot be mounted
#define REDFSMNT_FMTALWAYS 0x2U // format always


//example how to start File system
/*
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
*/
void foo(void *p)
{
	//remove compiler warning about unused parameters
	(void)p;

	//can use either VOL0 or VOL1 
	int fd = red_open("VOL0:/newFile", RED_O_RDWR | RED_O_CREAT);
	char buff[20];

	if (fd < 0)
	{
		switch (red_errno)
		{
		case RED_EINVAL:
			fprintf(stderr, "ulOpenMode is invalid; or pszPath is NULL ; or the volume containing the path is not mounted; or RED_O_CREAT is included in ulOpenMode , and the path ends with dot or dot-dot.\n");
			break;
		case RED_EEXIST:
			fprintf(stderr, "file already exists\n");
			break;
		}
		REDERROR();
	}

	int iErr = red_write(fd, "Some stuff here", 20);
	if (iErr < 0)
	{
		fprintf(stderr, "Write failed with err %d\n", (int)red_errno);
		exit(red_errno);
	}

	iErr = red_lseek(fd, 0, RED_SEEK_SET);
	if (iErr < 0)
	{
		fprintf(stderr, "Seek file failed %d\n", (int)red_errno);
		exit(red_errno);
	}

	iErr = red_read(fd, buff, 20);
	if (iErr < 0)
	{
		fprintf(stderr, "Write failed with err %d\n", (int)red_errno);
		exit(red_errno);
	}
	printf("%s\n", buff);
}

void vStartFSWriteTask(UBaseType_t uxPriority)
{

	xTaskCreate(foo, "foo", configMINIMAL_STACK_SIZE, NULL, uxPriority, (TaskHandle_t *)NULL);
}
