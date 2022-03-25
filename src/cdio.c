#include <sysio.h>
#include <ucm.h>
#include <cdfm.h>
#include <stdio.h>

#include "cdio.h"
#include "graphics.h"

int readData()
{
	int file = open("DATA.RTF", READ_);
	int res = file;

	res |= readScreen(file);

	close(file);
	return res;
}

void safeRead(readFunc)
	int (*readFunc)();
{
	int retry = RETRY_COUNT;
	while (1) {	
		if (readFunc() > 0) return;
		if (retry-- == 0) {
			/*discError();
			retry = RETRY_COUNT;
			*/
			_exit(0);
		}
	}
}