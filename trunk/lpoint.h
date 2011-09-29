#ifndef LIST_POINT_H
#define LIST_POINT_H
#include "point.h"
#include "drawing.h"
typedef struct telement_list* address;
typedef struct telement_list{
	point* p;
	address next_element, prev_element;
} element_polygon;

typedef struct{
	address first,last;
} polygon;

void insert_point(polygon* L, point* element);
polygon* ctor_polygon();
void init_polygon(polygon* L);
void dtor_list_point(polygon* L);
void print_list_point(polygon* L);
void draw_polygon(polygon* L);
void copy_polygon(polygon* dest, polygon* src);
void rotate_polygon(polygon* p, double radian);
void translate_polygon(polygon* p, int x, int y);
void scale_polygon(polygon* p, int x, int y);
#endif