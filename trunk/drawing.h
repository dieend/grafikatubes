#ifndef DRAWING_H
#define DRAWING_H

#define INT_VIDEO			0x10
#define SET_MODE			0x00
#define MODE_TEXT			0x03 /* Text mode */
#define MODE_VGA			0x13 /* Graphics mode 320x200 256 colors */
#define SCREEN_WIDTH		320
#define SCREEN_HEIGHT		200
#define sgn(x) ((x<0)?-1:((x>0)?1:0))

#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

#include <stdlib.h>
#include <dos.h>
#include <mem.h>

typedef unsigned char  byte;
typedef unsigned short word;

extern byte *double_buffer;
extern const byte *VGA;
extern const word *my_clock;

void setMode(byte mode);
void setPixel(int x, int y, byte color);
byte getPixel(int x, int y);
void setLine(int x1, int y1, int x2, int y2, byte color);
void setRect(int left,int top, int right, int bottom, byte color);
void fillRect(int left,int top, int right, int bottom, byte color);
void clearBuffer(byte *buffer);
void waitForRetrace(void);
void showBuffer(byte *buffer);
void start_mode_vga();
void exit_mode_vga();

#endif