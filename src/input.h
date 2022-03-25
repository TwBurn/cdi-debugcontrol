#ifndef __INPUT_H__
#define	__INPUT_H__

typedef struct InputRelative {
	short DeltaX;
	short DeltaY;
	short Buttons;
	short Delay;
} InputRelative;

extern int inputPath1, inputPath2;

extern InputRelative ipResult1, ipResult2;

void updateInput1();
void updateInput2();
void updateInput();

void clearInput();

#endif