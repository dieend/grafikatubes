#include "drawing.h"

byte *double_buffer;
const byte *VGA = (byte *)0xA0000000L;
const word *my_clock=(word *)0x0000046C;

void start_mode_vga(){
	if ((double_buffer = (unsigned char *) malloc(SCREEN_WIDTH*SCREEN_HEIGHT)) == NULL) {
		printf("Not enough memory to allocate double buffer\n");
		exit(1);
	}
	setMode(MODE_VGA);
}

void exit_mode_vga(){
	clearBuffer(double_buffer);
	showBuffer(double_buffer);
	free(double_buffer);
	setMode(MODE_TEXT);
}

void clearBuffer(byte* buffer){
	memset(buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void showBuffer(byte *buffer)
{
	#ifdef VERTICAL_RETRACE
		while ((inp(INPUT_STATUS_1) & VRETRACE));
		while (!(inp(INPUT_STATUS_1) & VRETRACE));
	#endif
	memcpy(VGA,buffer,SCREEN_WIDTH * SCREEN_HEIGHT);
}

void setMode(byte mode){
	union REGS regs;
	
	regs.h.ah = SET_MODE;
	regs.h.al = mode;
	int86(INT_VIDEO,&regs,&regs);
}

void setPixel(int x, int y, byte color){
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		double_buffer[(y << 8) + (y << 6) + x] = color;
}

byte getPixel(int x, int y){
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		return double_buffer[(y << 8) + (y << 6) + x];
	else
		return 0;
}

void setLine(int x1, int y1, int x2, int y2, byte color){
	int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

	dx=x2-x1;      /* the horizontal distance of the line */
	dy=y2-y1;      /* the vertical distance of the line */
	dxabs=abs(dx);
	dyabs=abs(dy);
	sdx=sgn(dx);
	sdy=sgn(dy);
	x=dyabs>>1;
	y=dxabs>>1;
	px=x1;
	py=y1;

	setPixel(px,py,color);

	if (dxabs>=dyabs) /* the line is more horizontal than vertical */
	{
		for(i=0;i<dxabs;i++)
		{
			y+=dyabs;
			if (y>=dxabs)
			{
				y-=dxabs;
				py+=sdy;
			}
			px+=sdx;
			setPixel(px,py,color);
		}
	}
	else /* the line is more vertical than horizontal */
	{
		for(i=0;i<dyabs;i++)
		{
			x+=dxabs;
			if (x>=dyabs)
			{
				x-=dyabs;
				px+=sdx;
			}
			py+=sdy;
			setPixel(px,py,color);
		}
	}
}

void setRect(int left,int top, int right, int bottom, byte color){
	word top_offset,bottom_offset,i,temp;

	if (top>bottom)
	{
		temp=top;
		top=bottom;
		bottom=temp;
	}
	if (left>right)
	{
		temp=left;
		left=right;
		right=temp;
	}

	for(i=left;i<=right;i++)
	{
		setPixel(i, top, color);
		setPixel(i, bottom, color);
	}
	for(i=top;i<=bottom;i++)
	{
		setPixel(left, i, color);
		setPixel(right, i, color);
	}
}

void fillRect(int left,int top, int right, int bottom, byte color){
	word top_offset,bottom_offset,i,temp,width;

	if (top>bottom)
	{
		temp=top;
		top=bottom;
		bottom=temp;
	}
	if (left>right)
	{
		temp=left;
		left=right;
		right=temp;
	}

	top_offset=(top<<8)+(top<<6)+left;
	bottom_offset=(bottom<<8)+(bottom<<6)+left;
	width=right-left+1;

	for(i=top_offset;i<=bottom_offset;i+=SCREEN_WIDTH)
	{
		memset(&double_buffer[i],color,width);
	}
}

void waitForRetrace(void)
{
    /* wait until done with vertical retrace */
    while  ((inp(INPUT_STATUS) & VRETRACE));
    /* wait until done refreshing */
    while (!(inp(INPUT_STATUS) & VRETRACE));
}
