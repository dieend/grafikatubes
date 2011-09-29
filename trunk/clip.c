#include "boolean.h"
#include "clip.h"
#include "drawing.h"

int xmin = 25;
int ymin = 25;
int xmax = 150;
int ymax = 100;

int CLIP_WIDTH = 50;
int CLIP_HEIGHT = 50;

const int INSIDE	= 0;	/* 0000 */
const int LEFT		= 1;	/* 0001 */
const int RIGHT		= 2;	/* 0010 */
const int BOTTOM	= 4;	/* 0100 */
const int TOP		= 8;	/* 1000 */

/*	Compute the bit code for a point (x, y) using the clip rectangle
	bounded diagonally by (xmin, ymin), and (xmax, ymax)
*/
OutCode ComputeOutCode(int x, int y)
{
	OutCode code;

	code = INSIDE;			/* initialised as being inside of clip window */

	if (x < xmin)			/* to the left of clip window */
		code |= LEFT;
	else if (x > xmax)		/* to the right of clip window */
		code |= RIGHT;
	if (y < ymin)			/* below the clip window */
		code |= BOTTOM;
	else if (y > ymax)		/* above the clip window */
		code |= TOP;

	return code;
}

/*	Cohen–Sutherland clipping algorithm clips a line from
	P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
	diagonal from (xmin, ymin) to (xmax, ymax).
*/
void CohenSutherlandLineClipAndDraw(int x0, int y0, int x1, int y1, byte color)
{
	/* compute outcodes for P0, P1, and whatever point lies outside the clip rectangle */
	OutCode outcode0 = ComputeOutCode(x0, y0);
	OutCode outcode1 = ComputeOutCode(x1, y1);
	boolean accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {	/* Bitwise OR is 0. Trivially accept and get out of loop */
			accept = true;
			break;
		} else if (outcode0 & outcode1) {	/* Bitwise AND is not 0. Trivially reject and get out of loop */
			break;
		} else {
			/*	failed both tests, so calculate the line segment to clip
				from an outside point to an intersection with clip edge */
			int x, y;

			/*	At least one endpoint is outside the clip rectangle; pick it. */
			OutCode outcodeOut = outcode0? outcode0 : outcode1;

			/*	Now find the intersection point;
				use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0) */
			if (outcodeOut & TOP) {				/* point is above the clip rectangle */
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & BOTTOM) {	/* point is below the clip rectangle */
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {	/* point is to the right of clip rectangle */
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {		/* point is to the left of clip rectangle */
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}
			/*	Now we move outside point to intersection point to clip
				and get ready for next pass. */
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(x0, y0);
			} else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(x1, y1);
			}
		}
	}
	if (accept) {
		/* Following functions are left for implementation by user based on his platform(OpenGL/graphics.h etc.) */
		setLine(x0, y0, x1, y1, color);
	}
}

void setClipBounds(int x0, int y0, int x1, int y1) {
	xmin = x0;
	ymin = y0;
	xmax = x1;
	ymax = y1;
	CLIP_WIDTH = xmax - xmin + 1;
	CLIP_HEIGHT = ymax - ymin + 1;
}

void resizeClipBounds(int new_width, int new_height) {
	if (new_width > 0 && new_width < SCREEN_WIDTH && new_height > 0 && new_height < SCREEN_HEIGHT) {
		int centerX, centerY, newxmin, newxmax, newymin, newymax;
		
		centerX = (xmin + xmax) / 2;
		centerY = (ymin + ymax) / 2;
		
		newxmin = centerX - (new_width) / 2;
		newxmax = centerX + (new_width) / 2;
		newymin = centerY - (new_height) / 2;
		newymax = centerY + (new_height) / 2;
		
		if (newxmin > 0 && newxmax < SCREEN_WIDTH && newymin > 0 && newymax < SCREEN_HEIGHT) {
			CLIP_WIDTH = new_width;
			CLIP_HEIGHT = new_height;
			xmin = newxmin;
			xmax = newxmax;
			ymin = newymin;
			ymax = newymax;
		}
	}
}

void drawClipBounds(byte color) {
	setRect(xmin, ymin, xmax, ymax, color);
}

void setPixel2(int x, int y, byte color) {
	if (x > xmin && x < xmax && y > ymin && y < ymax)
		double_buffer[(y << 8) + (y << 6) + x] = color;
}

void fillRect2(int left,int top, int right, int bottom, byte color){
	word x,y,i,temp,width;

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
	
	for (y = top; y < bottom; y++) {
		CohenSutherlandLineClipAndDraw(left, y, right, y, color);
	}
}