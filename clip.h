#ifndef CLIP_H
#define CLIP_H

#include "drawing.h"
#include "font.h"
#include "boolean.h"

typedef int OutCode;

extern int xmin;
extern int ymin;
extern int xmax;
extern int ymax;

extern int CLIP_WIDTH;
extern int CLIP_HEIGHT;

extern const int INSIDE; /* 0000 */
extern const int LEFT;   /* 0001 */
extern const int RIGHT;  /* 0010 */
extern const int BOTTOM; /* 0100 */
extern const int TOP;    /* 1000 */

/* 	Compute the bit code for a point (x, y) using the clip rectangle
	bounded diagonally by (xmin, ymin), and (xmax, ymax)
*/
OutCode ComputeOutCode(int x, int y);

/*	Cohen–Sutherland clipping algorithm clips a line from
	P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
	diagonal from (xmin, ymin) to (xmax, ymax).
*/
void CohenSutherlandLineClipAndDraw(int x0, int y0, int x1, int y1, byte color);

boolean checkLineClip(int *x0, int *y0, int *x1, int *y1);
void drawLineClip(int x0, int y0, int x1, int y1, byte color);

void setClipBounds(int x0, int y0, int x1, int y1);
void resizeClipBounds(int new_width, int new_height);
void drawClipBounds(byte color);

void setPixel2(int x, int y, byte color);
void fillRect2(int left,int top, int right, int bottom, byte color);

boolean checkFontClip(FONT* font);
void drawFontClip(FONT* font, int zx0, int zy0, int zx1, int zy1);

#endif