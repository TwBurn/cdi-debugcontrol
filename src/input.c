#include <csd.h>
#include <sysio.h>
#include "input.h"

#define PT_CENTER 256
#define PT_DELAY 2

#define clamp(value, limit) (value < -limit ? -limit : (value > limit ? limit : value))

int inputPath1, inputPath2;

InputRelative ipResult1, ipResult2;

void initInput()
{
	u_char *devName;

	devName = (u_char*)csd_devname(DT_PTR, 1);
	if (devName) {
		inputPath1 = open(devName, UPDAT_);

		free(devName);

		if (inputPath1 >= 0) {
			pt_org(inputPath1, 0, 0);
		}
	}

	devName = (u_char*)csd_devname(DT_PTR, 2);
	if (devName) {
		inputPath2 = open(devName, UPDAT_);

		free(devName);

		if (inputPath2 >= 0) {
			pt_org(inputPath2, 0, 0);
		}
	}

	clearInput();

}

void closeInput()
{
	if (inputPath1 > 0) {
		close(inputPath1);
		inputPath1 = 0;
	}
	if (inputPath2 > 0) {
		close(inputPath2);
		inputPath2 = 0;
	}
}

void handleInput(path, result)
	int path;
	InputRelative *result;
{
	int x, y, buttons;

	if (path > 0) {
		pt_coord(path, &buttons, &x, &y);
		
		result->Buttons = buttons;

		x = x - PT_CENTER;
		y = y - PT_CENTER;

		x = clamp(x, 64);
		y = clamp(y, 64);

		if (x || y) {
			result->DeltaX = x;
			result->DeltaY = y;
			result->Delay  = 0;
			pt_pos(path, PT_CENTER, PT_CENTER);
		}
		else if ((result->Delay++) >= PT_DELAY) {
			result->DeltaX = 0;
			result->DeltaY = 0;
			result->Delay  = 0;
		}
	}
}


void updateInput1() {
	handleInput(inputPath1, &ipResult1);
}

void updateInput2() {
	handleInput(inputPath2, &ipResult2);
}

void updateInput()
{
	updateInput1();
	updateInput2();
}

void clearInput() {
	/* Clear buttons first, these will be set to their actual values by updateInput() */
	ipResult1.Buttons = 0;
	ipResult2.Buttons = 0;

	updateInput();
	
	ipResult1.DeltaX  = 0;
	ipResult1.DeltaY  = 0;
	ipResult1.Delay   = 0;

	ipResult2.DeltaX  = 0;
	ipResult2.DeltaY  = 0;
	ipResult2.Delay   = 0;
}