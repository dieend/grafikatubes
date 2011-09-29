#ifndef POINT_H
#define POINT_H
#include <math.h>
#include "drawing.h"
#define PI 3.14
#define POINT_STATUS_PENDOWN 0
#define POINT_STATUS_PENUP 1
#define POINT_STATUS_ 2

typedef struct{
	int x,y;
	int status;
} point;
double radian_from_degree(double degree);
void copy_point(point* dest, point* origin);
point* ctor_point(int x, int y,int status);
void dtor_point(point* p);

void rotate_point(point* p, double radian);
void translate_point(point* p, int x, int y);
void scale_point(point* p, int x, int y);
void draw_point(point* p);
void print_point(point* p);
#endif