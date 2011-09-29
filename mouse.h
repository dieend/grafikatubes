#ifndef MOUSE_H
#define MOUSE_H

#include "drawing.h"

#define INT_MOUSE           0x33

#define MOUSE_RESET         0x00
#define MOUSE_GETPRESS      0x05
#define MOUSE_GETRELEASE    0x06
#define MOUSE_SETSHAPE		0x09
#define MOUSE_GETMOTION     0x0B

#define LEFT_BUTTON         0x00
#define RIGHT_BUTTON        0x01
#define MIDDLE_BUTTON       0x02

#define MOUSE_WIDTH			3
#define MOUSE_HEIGHT		3
#define MOUSE_SIZE			(MOUSE_HEIGHT*MOUSE_WIDTH)

typedef short sword;

typedef struct
{
	byte on;
	byte button1;
	byte button2;
	byte button3;
	int num_buttons;
	sword x;
	sword y;
} MOUSE;

sword 	initMouse(MOUSE *mouse);
sword 	getMousePress(sword button);
sword 	getMouseRelease(sword button);
void 	getMouseMotion(sword *dx, sword *dy);
void 	drawMouse(MOUSE *mouse, byte color);

#endif