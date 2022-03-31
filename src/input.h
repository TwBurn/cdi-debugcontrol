#ifndef __INPUT_H__
#define	__INPUT_H__

typedef struct InputRelative {
	char DeltaX;
	char DeltaY;
	u_char Buttons;
	u_char Delay;
} InputRelative;

extern int inputPath1, inputPath2;

extern InputRelative ipResult1, ipResult2;

void updateInput1();
void updateInput2();
void updateInput();

void clearInput();

#endif