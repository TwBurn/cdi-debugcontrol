#define NODEVELOP

#include <csd.h>
#include <sysio.h>
#include <ucm.h>
#include <events.h>

#include <setsys.h>

#include "input.h"
#include "video.h"
#include "graphics.h"

#define LCT_ROW_START 2

#define cl_row(i) (((i >> 2) << 1) + LCT_ROW_START)
#define cl_col(i) ((i & 3) + 1)

#define cl_white(i) cp_clut(i, 255, 255, 255)
#define cl_blue(i)  cp_clut(i,  11,  94, 216)
#define cl_red(i)   cp_clut(i, 180,  32,  42)
#define cl_dgray(i) cp_clut(i,  38,  43,  68)
#define cl_lgray(i) cp_clut(i, 192, 203, 220)
#define cl_black(i) cp_clut(i,   0,   0,   0)

#define cl_wrli(i, c) dc_wrli(videoPath, lctB, cl_row(i), cl_col(i), c)

#define wr_white(i) cl_wrli(i, cl_white(i))
#define wr_blue(i)  cl_wrli(i, cl_blue(i))
#define wr_red(i)   cl_wrli(i, cl_red(i))
#define wr_dgray(i) cl_wrli(i, cl_dgray(i))

/* Origin positions of crosses */
#define ORG1 (163 * SCREEN_WIDTH + 89)
#define ORG2 (163 * SCREEN_WIDTH + 279)

#define dpos(x, y) (y * SCREEN_WIDTH + x)

void drawCross(pos, col)
	int pos;
	register u_char col;
{
	register u_char *drw = paCursor + pos;

	drw += 7;                  (*drw++) = col;(*drw++) = col;
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


int oldPos1 = 0, oldPos2 = 0;
void drawInputs()
{
	int newPos1, newPos2;
	updateInput();
	drawInput(&ipResult1, 3);
	drawInput(&ipResult2, 11);

	newPos1 = ORG1 + dpos(ipResult1.DeltaX, ipResult1.DeltaY);
	newPos2 = ORG2 + dpos(ipResult2.DeltaX, ipResult2.DeltaY);

	if (inputPath1 && oldPos1 != newPos1) {
		drawCross(oldPos1, 0);
		drawCross(newPos1, 30);
		oldPos1 = newPos1;
	}

	if (inputPath2 && oldPos2 != newPos2) {
		drawCross(oldPos2, 0);
		drawCross(newPos2, 30);
		oldPos2 = newPos2;
	}
}

void initProgram() {
	int i, c;
	/* Fill 8 rows of SET CLUT BANK in LCT */
	for (i = 0; i < 8; i++) {
		dc_wrli(videoPath, lctA, 2 * i + LCT_ROW_START, 0, cp_cbnk(0));
		dc_wrli(videoPath, lctB, 2 * i + LCT_ROW_START, 0, cp_cbnk(2));
	}
	
	for (i = 0; i < 32; i++) {
		switch(i) {
			case 0:  c = cl_black(i); break;
			case 25: c = cl_lgray(i); break;
			case 29: c = cl_blue(i);  break;
			case 30: c = cl_red(i);   break;
			default: c = cl_white(i); break;
		}

		dc_wrli(videoPath, lctA, cl_row(i), cl_col(i), c);
		dc_wrli(videoPath, lctB, cl_row(i), cl_col(i), c);
	}

	inputPath1 ? wr_blue(1) : wr_dgray(1);
	inputPath2 ? wr_blue(2) : wr_dgray(2);

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
