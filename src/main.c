#define NODEVELOP

#include <csd.h>
#include <sysio.h>
#include <ucm.h>
#include <events.h>
#include <stdio.h>
#include <setsys.h>

#include "input.h"
#include "video.h"
#include "graphics.h"
#include "cdio.h"

#define LCT_ROW_START 2

#define cl_row(i) (((i >> 2) << 1) + LCT_ROW_START)
#define cl_col(i) ((i & 3) + 1)

#define cl_white(i) cp_clut(i + 1, 255, 255, 255)
#define cl_blue(i)  cp_clut(i + 1,  11,  94, 216)
#define cl_red(i)   cp_clut(i + 1, 180,  32,  42)
#define cl_gray(i)  cp_clut(i + 1,  38,  43,  68)

#define cl_wrli(i, c) dc_wrli(videoPath, lctB, cl_row(i), cl_col(i), c)

#define wr_white(i) cl_wrli(i, cl_white(i))
#define wr_blue(i)  cl_wrli(i, cl_blue(i))
#define wr_red(i)   cl_wrli(i, cl_red(i))
#define wr_gray(i)  cl_wrli(i, cl_gray(i))

/* Origin positions of crosses */
#define ORG1 (163 * SCREEN_WIDTH + 89)
#define ORG2 (163 * SCREEN_WIDTH + 279)

#define dpos(x, y) (y * SCREEN_WIDTH + x)

void drawCross(pos, col)
	int pos;
	register u_char col;
{
	register u_char *drw = paCursor + pos;

	drw += 7;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  2);(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  2);(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  2);(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  3);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  4);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  6);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH - 12);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH - 16);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH - 12);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  6);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  4);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  4);(*drw++) = col;(*drw++) = col;(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  3);(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  2);(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  2);(*drw++) = col;(*drw++) = col;
	drw += (SCREEN_WIDTH -  2);(*drw++) = col;(*drw++) = col;
}

void drawInput(input, start)
	InputRelative *input;
	u_char start;
{
	int i;
	
	for (i = 0; i < 8; i++) {
		if (input->Buttons & (1 << i)) {
			wr_red(start + i);
		}
		else {
			wr_white(start + i);
		}
	}
}

void drawInputs()
{
	if (inputPath1) drawCross(ORG1 + dpos(ipResult1.DeltaX, ipResult1.DeltaY), 0);
	if (inputPath2) drawCross(ORG2 + dpos(ipResult2.DeltaX, ipResult2.DeltaY), 0);

	updateInput();
	drawInput(&ipResult1, 2);
	drawInput(&ipResult2, 10);

	if (inputPath1) drawCross(ORG1 + dpos(ipResult1.DeltaX, ipResult1.DeltaY), 30);
	if (inputPath2) drawCross(ORG2 + dpos(ipResult2.DeltaX, ipResult2.DeltaY), 30);
}

void initProgram() {
	int i;
	readData();

	/* Fill 5 rows of SET CLUT BANK in LCT */
	for (i = 0; i < 5; i++) {
		dc_wrli(videoPath, lctB, 2 * i + LCT_ROW_START, 0, cp_cbnk(2));
	}
	
	cl_wrli(0, inputPath1 ? cl_blue(0) : cl_gray(0));
	cl_wrli(1, inputPath2 ? cl_blue(1) : cl_gray(1));

	drawInputs();	

	setIcf(ICF_MAX, ICF_MAX);
}

void initSystem()
{
	initVideo();
	initGraphics();
	initInput();
	initProgram();
}

void closeSystem()
{
	closeVideo();
	closeInput();
}

void runProgram() {
	int evId = _ev_link("line_event");
	
	while(1) {
		drawInputs();

		_ev_wait(evId, 1, 1); /* Wait for VBLANK */
	}
}

int main(argc, argv)
	int argc;
	char* argv[];
{
	int res;

	initSystem();
	runProgram();
	closeSystem();
	exit(0);
}
