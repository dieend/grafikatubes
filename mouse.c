#include "mouse.h"

sword initMouse(MOUSE *mouse)
{
	union REGS in, out;

	in.x.ax = MOUSE_RESET;
	int86(INT_MOUSE, &in, &out);
	
	mouse->on = out.x.ax;
	mouse->num_buttons = out.x.bx;
	mouse->button1 = 0;
	mouse->button2 = 0;
	mouse->button3 = 0;
	mouse->x = SCREEN_WIDTH/2;
	mouse->y = SCREEN_HEIGHT/2;
	
	return mouse->on;
}

sword getMousePress(sword button)
{
	union REGS in, out;

	in.x.ax = MOUSE_GETPRESS;
	in.x.bx = button;
	int86(INT_MOUSE, &in, &out);
	return out.x.bx;
}

sword getMouseRelease(sword button)
{
	union REGS in, out;

	in.x.ax = MOUSE_GETRELEASE;
	in.x.bx = button;
	int86(INT_MOUSE, &in, &out);
	return out.x.bx;
}

void getMouseMotion(sword *dx, sword *dy)
{
	union REGS in, out;

	in.x.ax = MOUSE_GETMOTION;
	int86(INT_MOUSE, &in, &out);
	*dx = out.x.cx;
	*dy = out.x.dx;
}

void showMouse(MOUSE *mouse, byte color)
{
	int mx = mouse->x;
	int my = mouse->y;
	
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