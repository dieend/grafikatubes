#include "mouse.h"

void getMouseMotion(sword *dx, sword *dy)
{
	union REGS regs;

	regs.x.ax = MOUSE_GETMOTION;
	int86(INT_MOUSE, &regs, &regs);
	*dx=regs.x.cx;
	*dy=regs.x.dx;
}

sword initMouse(MOUSE *mouse)
{
	sword dx,dy;
	union REGS regs;

	regs.x.ax = MOUSE_RESET;
	int86(INT_MOUSE, &regs, &regs);
	mouse->on=regs.x.ax;
	mouse->num_buttons=regs.x.bx;
	mouse->button1=0;
	mouse->button2=0;
	mouse->button3=0;
	mouse->x=SCREEN_WIDTH/2;
	mouse->y=SCREEN_HEIGHT/2;
	getMouseMotion(&dx,&dy);
	return mouse->on;
}

sword getMousePress(sword button)
{
	union REGS regs;

	regs.x.ax = MOUSE_GETPRESS;
	regs.x.bx = button;
	int86(INT_MOUSE, &regs, &regs);
	return regs.x.bx;
}

sword getMouseRelease(sword button)
{
	union REGS regs;

	regs.x.ax = MOUSE_GETRELEASE;
	regs.x.bx = button;
	int86(INT_MOUSE, &regs, &regs);
	return regs.x.bx;
}

void showMouse(MOUSE *mouse, byte color)
{
	int mx = mouse->x;
	int my = mouse->y;

	mouse->under[0] = getPixel(mx, my);
	mouse->under[1] = getPixel(mx+0, my+1);
	mouse->under[2] = getPixel(mx+0, my+2);
	mouse->under[3] = getPixel(mx+1, my+0);
	mouse->under[4] = getPixel(mx+1, my+1);
	mouse->under[5] = getPixel(mx+1, my+2);
	mouse->under[6] = getPixel(mx+2, my+0);
	mouse->under[7] = getPixel(mx+2, my+1);
	mouse->under[8] = getPixel(mx+2, my+2);
	setPixel(mx, my, color);
	setPixel(mx+0, my+1, color);
	setPixel(mx+0, my+2, color);
	setPixel(mx+1, my+0, color);
	setPixel(mx+1, my+1, color);
	setPixel(mx+1, my+2, color);
	setPixel(mx+2, my+0, color);
	setPixel(mx+2, my+1, color);
	setPixel(mx+2, my+2, color);
}

void hideMouse(MOUSE *mouse)
{
	int mx = mouse->x;
	int my = mouse->y;

	setPixel(mx, my, mouse->under[0]);
	setPixel(mx+0, my+1, mouse->under[1]);
	setPixel(mx+0, my+2, mouse->under[2]);
	setPixel(mx+1, my+0, mouse->under[3]);
	setPixel(mx+1, my+1, mouse->under[4]);
	setPixel(mx+1, my+2, mouse->under[5]);
	setPixel(mx+2, my+0, mouse->under[6]);
	setPixel(mx+2, my+1, mouse->under[7]);
	setPixel(mx+2, my+2, mouse->under[8]);
}