#include "mouse.h"
#include "drawing.h"
#include <conio.h>
#include <stdio.h>

#define PICKER_BOX_SIZE		8
#define PICKER_GRID_SIZE	16 /* 16*16=256 colors */
#define PICKER_SIZE			(PICKER_BOX_SIZE*PICKER_GRID_SIZE)
#define MARGIN				16

int main(){
	int i, j, k, x, y, x2, y2, color, top, left, right, down, done;
	float t1, t2, t3;
	word last_time, redraw;
	word press = 0, release;
	sword dx = 0, dy = 0, new_x, new_y;
	MOUSE mouse;
	byte mouseColor, pickedColor;

	start_mode_vga();
	
	if (!initMouse(&mouse))            /* init mouse */
	{
		printf("Mouse not found.\n");
		exit(1);
	}

	new_x=mouse.x;
	new_y=mouse.y;
	redraw=0xFFFF;
	mouseColor = 15;
	pickedColor = 0;
	showMouse(&mouse, mouseColor);
	last_time = *my_clock;

		done = 0;
	while (!done) {
		
		if (redraw) {
			clearBuffer(double_buffer);
			
			waitForRetrace();
			hideMouse(&mouse);
			
			/* Draw border */
			top = 1; left = 1; right = 2; down = 2; color = 13;
			setRect(MARGIN - left, MARGIN - top, MARGIN + right + PICKER_SIZE, MARGIN + down + PICKER_SIZE, color);
			
			/* Draw colors grid */
			for (i = 0; i < PICKER_GRID_SIZE; i++) {
				for (j = 0; j < PICKER_GRID_SIZE; j++) {
					color = i + j * PICKER_GRID_SIZE;
					x = MARGIN + 1 + i * PICKER_BOX_SIZE;
					y = MARGIN + 1 + j * PICKER_BOX_SIZE;
					x2 = x + PICKER_BOX_SIZE - 1;
					y2 = y + PICKER_BOX_SIZE - 1;
					fillRect(x, y, x2, y2, color);
				}
			}
			
			if (press == 1 &&
					new_x > MARGIN && new_x <= MARGIN + PICKER_SIZE &&
					new_y > MARGIN && new_y <= MARGIN + PICKER_SIZE) {
					
				pickedColor = getPixel(new_x, new_y);
			}
			
			/* Draw picked color */
			fillRect( 2 * MARGIN + PICKER_SIZE, MARGIN, 2 * MARGIN + PICKER_SIZE + 20, MARGIN + 20, pickedColor);
			
			mouse.x = new_x;
			mouse.y = new_y;
			showMouse(&mouse, mouseColor);
			last_time = *my_clock;
			redraw = 0;
		}
		
		do {
			getMouseMotion(&dx,&dy);
			press   = getMousePress(LEFT_BUTTON);
			release = getMouseRelease(LEFT_BUTTON);
		} while (dx==0 && dy==0 && press==0 && release==0 && *my_clock==last_time);
		
		
		if (press) {
			mouse.button1=1; redraw = 1;
		}
		if (release) mouse.button1=0;
		
		if (dx || dy)
		{
			new_x = mouse.x+dx;
			new_y = mouse.y+dy;
			if (new_x<0)   new_x=0;
			if (new_y<0)   new_y=0;
			if (new_x>319) new_x=319;
			if (new_y>199) new_y=199;
			redraw = 1;
		}
		
		if (new_x < 5 && new_y < 5 && mouse.button1 == 1)
			done = 1;
		
		showBuffer(double_buffer);
	}
	clearBuffer(double_buffer);
	showBuffer(double_buffer);
	exit_mode_vga();
	printf("Press any key to continue...\n");
	getch();
	
	return 0;
}