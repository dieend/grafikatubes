#ifndef FONT_H
#define FONT_H
#include "lpoint.h"
#include <stdio.h>
#define FONT_HEIGHT 20

typedef struct{
	char type;
	byte height,width;
	int x,y;
	polygon* points;
} FONT;

extern FONT* basic_font[256];

FONT* set_font(char type, int x, int y);
FONT* ctor_font(char type, int x,int y);
void dtor_font(FONT* font);
void rotate_font(FONT* font, double radian);
void translate_font(FONT* font, int x, int y);
void scale_font(FONT* font, int x, int y);
void draw_font(FONT* font);
void init_font_db(char* file_name);
void clear_font_db();
#endif