#include <sysio.h>
#include <ucm.h>
#include <stdio.h>
#include <memory.h>
#include "video.h"
#include "graphics.h"
#include "data.h"

u_int frameDone = 0, frameTick = 0;

u_char *paCursor;
u_char *pbBackground;

int curIcfA = ICF_MAX;
int curIcfB = ICF_MAX;

void fillBuffer(buffer, data, size)
	register u_int *buffer;
	register u_int data, size;
{
	int i;
	size = size >> 2;
	for (i = 0; i < size; i++) {
		*buffer++ = data;
	}
}

void fillVideoBuffer(videoBuffer, data)
	register u_int *videoBuffer;
	u_int data;
{
	fillBuffer(videoBuffer, data, VBUFFER_SIZE);
}

void createVideoBuffers()
{
	setIcf(ICF_MIN, ICF_MIN);
	paCursor     = (u_char*)srqcmem(VBUFFER_SIZE, VIDEO1);
	pbBackground = (u_char*)srqcmem(VBUFFER_SIZE, VIDEO2);

	fillVideoBuffer(paCursor, 0);
	decodeRle(backgroundData, pbBackground);

	dc_wrli(videoPath, lctA, 0, 0, cp_dadr((int)paCursor + pixelStart));
	dc_wrli(videoPath, lctB, 0, 0, cp_dadr((int)pbBackground + pixelStart));
}

void readPalette(plane, filename)
    int plane;
    u_char *filename;
{
    FILE *file = fopen(filename, "r");
	fread(fctBuffer, sizeof(int), 130, file);
	fclose(file);
	dc_wrfct(videoPath, (plane == PA) ? fctA : fctB, FCT_PAL_START, 130, fctBuffer);
}

int readImage(file, videoBuffer)
    int file;
	u_char *videoBuffer;
{
	return read(file, videoBuffer, VBUFFER_SIZE);
}


int readScreen(file)
	int file;
{
	return read(file, pbBackground, 6144);
}

void copyRect(sourceBuffer, targetBuffer, x, y, width, height, sourceWidth)
	u_char *sourceBuffer, *targetBuffer;
	u_short x, y, width, height, sourceWidth;
{
	register u_char* dst = targetBuffer + y * SCREEN_WIDTH + x;
	register u_char* src = sourceBuffer;
	register u_short h, w;
	register u_char tmp;

	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			tmp = *src++; if (tmp) { *dst = tmp; } dst++;
		}
		dst += SCREEN_WIDTH - width;
		src += sourceWidth - width;
	}
}

void clearRect(videoBuffer, x, y, width, height, color)
	u_char *videoBuffer;
	u_short x, y, width, height;
	u_char color;
{
	register u_int value = (color << 24) | (color << 16) | (color << 8) | color;
	register u_int* dst = (u_int*)(videoBuffer + y * SCREEN_WIDTH + x);
	register u_short h, w;

	width >>= 2;

	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) *dst++ = value;
		dst += (SCREEN_WIDTH >> 2) - width;
	}

}

void decodeRle(source, target)
	register u_char* source;
	register u_char* target;
{
	register int x, y;
	register u_char count, value;

	for (y = 0; y < SCREEN_HEIGHT; y++) {
		x = SCREEN_WIDTH;

		while (x) {
			value = *source++;

			if (value & 0x80) {
				value &= 0x7F;
				count = *source++;
				if (count) {
					while(count--) {
						*target++ = value; x--;
					}
				}
				else {
					while (x) {
						*target++ = value; x--;
					}
				}
			}
			else {
				*target++ = value; x--;
			}
		}
	}
}


void setIcf(icfA, icfB)
	register int icfA, icfB;
{
	curIcfA = icfA > ICF_MAX ? ICF_MAX : (icfA < ICF_MIN ? ICF_MIN : icfA);
	curIcfB = icfB > ICF_MAX ? ICF_MAX : (icfB < ICF_MIN ? ICF_MIN : icfB);

	dc_wrli(videoPath, lctA, 0, 7, cp_icf(PA, curIcfA));
	dc_wrli(videoPath, lctB, 0, 7, cp_icf(PB, curIcfB));
}

void initGraphics()
{
	createVideoBuffers();
}