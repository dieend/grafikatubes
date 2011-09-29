#include <conio.h>
#include "clip.h"
#include "drawing.h"
#include "mouse.h"
#include "font.h"

#define CLIP_STATIC 0
#define CLIP_FOLLOW 1

#define CLIP_COLOR_NORMAL 8
#define CLIP_COLOR_FOCUS 4

/*
#define CLIP_X0 50
#define CLIP_Y0 50
#define CLIP_X1 (CLIP_X0+CLIP_WIDTH)
#define CLIP_Y1 (CLIP_Y0+CLIP_HEIGHT)
*/

#define PICKER_BOX_SIZE		8
#define PICKER_GRID_SIZE	16 /* 16*16=256 colors */
#define PICKER_SIZE			(PICKER_BOX_SIZE*PICKER_GRID_SIZE)
#define MARGIN				16

/* Zoomed clipping region variables */
int zx0, zy0, zx1, zy1;

void checkDrawFontClip(FONT *E);
void recheck_new_mouse_pos(int clipMode, sword *new_x, sword *new_y);

int main() {
	/* Variables */
	char c;
	int done, clipMode;
	word last_time, redraw;
	
	/* Mouse variables */
	MOUSE mouse;
	word press, release;
	sword new_x, new_y, dx, dy;
	byte mouseColor;
	
	/* Font variables */
	FONT *E, *F, *G, *H;
	
	zx0 = 180; zy0 = 20;
	
	/* Init font database */
	init_font_db("data.txt");
	
	/* Init fonts */
	E = set_font('E', 20, 60);
	F = set_font('F', 40, 60);
	G = set_font('G', 20, 120);
	H = set_font('H', 40, 120);
	
	/* Init VGA mode first */
	start_mode_vga();
	clearBuffer(double_buffer);
	showBuffer(double_buffer);
	
	/* Init mouse */
	if (!initMouse(&mouse))
	{
		printf("Mouse not found.\n");
		exit(1);
	}
	
	/* Get mouse position */
	new_x = mouse.x;
	new_y = mouse.y;
	/* Show mouse */
	mouseColor = 15;
	showMouse(&mouse, mouseColor);
	last_time = *my_clock;
	
	clipMode = CLIP_FOLLOW;
	setClipBounds(xmin, ymin, xmax, ymax);
	
	redraw = 1;
	done = 0;
	
	/* Main loop */
	while (!done) {
		/* Draw function */
		if (redraw) {
			int x1, y1, x2, y2, i, j;
			byte color;
			/* Clear the buffer first */
			clearBuffer(double_buffer);
			waitForRetrace();
			
			/*** Define what to draw here ***/
			
			/* Draw text */
			draw_font(E);
			draw_font(F);
			draw_font(G);
			draw_font(H);
			
			/* Draw CLIP cursor rectangle bound */
			drawClipBounds(clipMode == CLIP_STATIC ? CLIP_COLOR_NORMAL : CLIP_COLOR_FOCUS);
			
			/* Draw a line */
			/*CohenSutherlandLineClipAndDraw(-8,24,145,244,6);*/
			drawLineClip(-8,24,145,244,6);
			
			/* Draw zoomer rectangle bound */
			zx1 = zx0 + abs(xmax - xmin) * 3;
			zy1 = zy0 + abs(ymax - ymin) * 3;
			setRect(zx0, zy0, zx1, zy1, 4);
			setPixel(2, 60, 4);
			
			/* Draw clipped fonts */
			/* Font E */
			checkDrawFontClip(E);
			checkDrawFontClip(F);
			checkDrawFontClip(G);
			checkDrawFontClip(H);
			/* Draw inside zoomer from cursor clipping region */
			/* Just translate and scale it */
			/*
			x1 = xmin; x2 = xmax;
			y1 = ymin; y2 = ymax;
			for (i = y1; i < y2; i++) {
				for (j = x1; j < x2; j++) {
					byte pixel = getPixel(j, i);
					setPixel(zx0 + (j - xmin) * 3, zy0 + (i - ymin) * 3, pixel);
				}
			}/*
			
			/*** End of draw function ***/
			
			/* Set mouse position */
			mouse.x = new_x;
			mouse.y = new_y;
			/* Draw only if CLIP_STATIC */
			if (clipMode == CLIP_STATIC) showMouse(&mouse, mouseColor);
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
			
			recheck_new_mouse_pos(clipMode, &new_x, &new_y);
			
			if (clipMode == CLIP_FOLLOW) {
				int x0 = new_x - CLIP_WIDTH / 2;
				int y0 = new_y - CLIP_HEIGHT / 2;
				int x1 = new_x + CLIP_WIDTH / 2;
				int y1 = new_y + CLIP_HEIGHT / 2;
				
				setClipBounds(x0, y0, x1, y1);
			}
			redraw = 1;
		}
		
		if (kbhit()) {
			c = getch();
			if (c == '1') {
				int x0, y0, x1, y1;
				clipMode = CLIP_STATIC;
				recheck_new_mouse_pos(clipMode, &new_x, &new_y);
				
				x0 = new_x - CLIP_WIDTH / 2;
				y0 = new_y - CLIP_HEIGHT / 2;
				x1 = new_x + CLIP_WIDTH / 2;
				y1 = new_y + CLIP_HEIGHT / 2;
				
				setClipBounds(x0, y0, x1, y1);
			}
			if (c == '2') { clipMode = CLIP_FOLLOW; recheck_new_mouse_pos(clipMode, &new_x, &new_y); }
			if (c == ']') { resizeClipBounds(CLIP_WIDTH + 2, CLIP_HEIGHT + 2); recheck_new_mouse_pos(clipMode, &new_x, &new_y); }
			if (c == '[') { resizeClipBounds(CLIP_WIDTH - 2, CLIP_HEIGHT - 2); recheck_new_mouse_pos(clipMode, &new_x, &new_y); }
			if (c == 27) done = 1; /* ESCAPE */
			redraw = 1;
		}
		
		showBuffer(double_buffer);
	}
	
	/* Delete fonts */
	dtor_font(E);
	dtor_font(F);
	dtor_font(G);
	dtor_font(H);
	
	/* Clear buffer and exit VGA mode */
	clearBuffer(double_buffer);
	showBuffer(double_buffer);
	exit_mode_vga();
	
	/* Closing */
	printf("Press any key to continue...\n");
	c = getch();
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

void checkDrawFontClip(FONT *E) {
	if (checkFontClip(E)) {
		FONT *t;
		t = set_font(E->type, zx0 + (E->x - xmin) * 3, zy1 + (E->y - ymax) * 3);
		scale_font(t, 3, 3);
		drawFontClip(t,zx0, zy0, zx1, zy1);
		dtor_font(t);
	}
}