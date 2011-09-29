#ifndef BITMAP_H
#define BITMAP_H
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef struct tBITMAP {
	word width;
	word height;
	byte *data;
} BITMAP;

void fskip(FILE *fp, int num_bytes);
void load_bitmap(char *file, BITMAP *b);
void draw_bitmap(BITMAP *bmp,int x,int y);
void draw_transparent_bitmap(BITMAP *bmp,int x,int y);

#endif