#include <conio.h>
#include "clip.h"
#include "drawing.h"
#include "mouse.h"

#define CLIP_STATIC 0
#define CLIP_FOLLOW 1

#define CLIP_WIDTH 50
#define CLIP_HEIGHT 50

/*
#define CLIP_X0 50
#define CLIP_Y0 50
#define CLIP_X1 (CLIP_X0+CLIP_WIDTH)
#define CLIP_Y1 (CLIP_Y0+CLIP_HEIGHT)
*/

void recheck_new_mouse_pos(int clipMode, sword *new_x, sword *new_y);

int main() {
	/* Variables */
	char c;
	int done, clipMode;
	word last_time, redraw;
	
	/* Clip variables */
	int clip_x0, clip_y0, clip_x1, clip_y1;
	
	/* Mouse variables */
	MOUSE mouse;
	word press, release;
	sword new_x, new_y, dx, dy;
	byte mouseColor;
	
	/* Init VGA mode first */
	start_mode_vga();
	
	/* Init mouse */
	if (!initMouse(&mouse))
	{
		printf("Mouse not found.\n");
		exit(1);
	}
	
	clearBuffer(double_buffer);
	showBuffer(double_buffer);
	
	clipMode = CLIP_FOLLOW;
	clip_x0 = 50; clip_y0 = 50;
	clip_x1 = clip_x0 + CLIP_WIDTH;
	clip_y1 = clip_y0 + CLIP_HEIGHT;
	
	new_x = mouse.x;
	new_y = mouse.y;
	redraw = 0xFFFF;
	mouseColor = 15;
	showMouse(&mouse, mouseColor);
	last_time = *my_clock;

	done = 0;
	
	/* Main loop */
	while (!done) {
		/* Draw function */
		if (redraw) {
			/* Clear the buffer first */
			clearBuffer(double_buffer);
			waitForRetrace();
			/* Hide mouse */
			hideMouse(&mouse);
			
			/*** Define what to draw here ***/
			if (clipMode == CLIP_FOLLOW) {
				xmin = new_x - CLIP_WIDTH / 2;
				ymin = new_y - CLIP_HEIGHT / 2;
				xmax = new_x + CLIP_WIDTH / 2;
				ymax = new_y + CLIP_HEIGHT / 2;
			} else if (clipMode == CLIP_STATIC) {
				xmin = clip_x0;
				ymin = clip_y0;
				xmax = clip_x1;
				ymax = clip_y1;
			}
			setRect(xmin, ymin, xmax, ymax, 8);
			
			/* Draw a line */
			CohenSutherlandLineClipAndDraw(-8,24,145,244);
			
			/*** End of draw function ***/
			
			/* Redraw and show mouse */
			mouse.x = new_x;
			mouse.y = new_y;
			showMouse(&mouse, mouseColor);
			last_time = *my_clock;
			redraw = 0;
		}
		
		/* Check mouse states */
		do {
			getMouseMotion(&dx,&dy);
			press   = getMousePress(LEFT_BUTTON);
			release = getMouseRelease(LEFT_BUTTON);
		} while (dx==0 && dy==0 && press==0 && release==0 && *my_clock==last_time);
		/* Check mouse button states */
		if (press) {
			mouse.button1=1; redraw = 1;
		}
		if (release) mouse.button1=0;
		/* Check mouse movement. Only redraw when mouse has moved */
		if (dx || dy)
		{
			new_x = mouse.x + dx;
			new_y = mouse.y + dy;
			/*
			if (clipMode == CLIP_FOLLOW) {
				if (new_x < CLIP_WIDTH / 2) new_x = CLIP_WIDTH / 2;
				if (new_y < CLIP_HEIGHT / 2) new_y = CLIP_HEIGHT / 2;
				if (new_x > SCREEN_WIDTH - CLIP_WIDTH / 2 - 1) new_x = SCREEN_WIDTH - CLIP_WIDTH / 2 - 1;
				if (new_y > SCREEN_HEIGHT - CLIP_HEIGHT / 2 - 1) new_y = SCREEN_HEIGHT - CLIP_HEIGHT / 2 - 1;
			} else {
				if (new_x < 0) new_x = 0;
				if (new_y < 0) new_y = 0;
				if (new_x > SCREEN_WIDTH - MOUSE_WIDTH) new_x = SCREEN_WIDTH - MOUSE_WIDTH;
				if (new_y > SCREEN_HEIGHT - MOUSE_HEIGHT) new_y = SCREEN_HEIGHT - MOUSE_HEIGHT;
			}
			*/
			recheck_new_mouse_pos(clipMode, &new_x, &new_y);
			redraw = 1;
		}
		
		if (kbhit()) {
			c = getch();
			if (c == '1') {
				clipMode = CLIP_STATIC;
				recheck_new_mouse_pos(clipMode, &new_x, &new_y);
				clip_x0 = new_x - CLIP_WIDTH / 2;
				clip_y0 = new_y - CLIP_HEIGHT / 2;
				clip_x1 = new_x + CLIP_WIDTH / 2;
				clip_y1 = new_y + CLIP_HEIGHT / 2;
			}
			if (c == '2') { clipMode = CLIP_FOLLOW; recheck_new_mouse_pos(clipMode, &new_x, &new_y); }
			if (c == 27) done = 1; /* ESCAPE */
			redraw = 1;
		}
		
		showBuffer(double_buffer);
	}
	
	/* Clear buffer and exit VGA mode */
	clearBuffer(double_buffer);
	showBuffer(double_buffer);
	exit_mode_vga();
	
	/* Closing */
	printf("Press any key to continue...\n");
	getch();
	printf("Program exit\n");
	return 0;
}

void recheck_new_mouse_pos(int clipMode, sword *new_x, sword *new_y) {
	if (clipMode == CLIP_FOLLOW) {
		if (*new_x < CLIP_WIDTH / 2) *new_x = CLIP_WIDTH / 2;
		if (*new_y < CLIP_HEIGHT / 2) *new_y = CLIP_HEIGHT / 2;
		if (*new_x > SCREEN_WIDTH - CLIP_WIDTH / 2 - 1) *new_x = SCREEN_WIDTH - CLIP_WIDTH / 2 - 1;
		if (*new_y > SCREEN_HEIGHT - CLIP_HEIGHT / 2 - 1) *new_y = SCREEN_HEIGHT - CLIP_HEIGHT / 2 - 1;
	} else {
		if (*new_x < 0) *new_x = 0;
		if (*new_y < 0) *new_y = 0;
		if (*new_x > SCREEN_WIDTH - MOUSE_WIDTH) *new_x = SCREEN_WIDTH - MOUSE_WIDTH;
		if (*new_y > SCREEN_HEIGHT - MOUSE_HEIGHT) *new_y = SCREEN_HEIGHT - MOUSE_HEIGHT;
	}
}