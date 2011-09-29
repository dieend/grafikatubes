#include "point.h"

double radian_from_degree(double degree){
	return (PI*degree/180);
}

void copy_point(point* dest, point* origin){
	dest->x = origin->x;
	dest->y = origin->y;
	dest->status = origin->status;
}
point* ctor_point(int x, int y, int status){
	point* p = (point*) malloc(1 * sizeof(point));
	p->x = x;
	p->y = y;
	p->status = status;
	return p;
}

void dtor_point(point* p){
	free(p);
}
void rotate_point(point* p, double radian){
}
void translate_point(point* p,int x, int y){
    p->x += x;
    p->y += y;
}
void scale_point(point* p, int x, int y, int ox, int oy){
    p->x = (p->x - ox) * x + ox;
    p->y = (p->y - oy) * y + oy;
}
void draw_point(point* p){
    /* buat warnanya */
    setPixel(p->x,p->y,15);
}
void print_point(point* p){
	if (p->status == POINT_STATUS_PENDOWN){
		printf("point = (%d,%d,down)",p->x, p->y);
	} else if (p->status == POINT_STATUS_PENUP){
		printf("point = (%d,%d,up)",p->x, p->y);
	} else {
		printf("point = (%d,%d,?)",p->x, p->y);
	}
}