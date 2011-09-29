#include "lpoint.h"

polygon* ctor_polygon(){
	polygon* L = (polygon*) malloc(sizeof(L));
	L->first = NULL;
	L->last = NULL;
	return L;
}

void init_polygon(polygon* L){
	L->first = NULL;
	L->last = NULL;
}

void insert_point(polygon* L, point* element) {
	if (L->first == NULL) {
		L->first = (address) malloc(sizeof(element_polygon));
		L->first->prev_element = NULL;
		L->first->next_element = NULL;
		L->first->p = element;
		L->last = L->first;
	} else {
		L->last->next_element = (address) malloc(sizeof(element_polygon));
		L->last->next_element->p = element;
		L->last->next_element->prev_element = L->last;
		L->last->next_element->next_element = NULL;
		L->last = L->last->next_element;
	}
}

void dtor_list_point(polygon* L){
	address it = L->first;
	while (it != L->last){
		if (it->prev_element != NULL) free(it->prev_element);
		it = it->next_element;
	}
	free(L->last);
	
}
void print_list_point(polygon* L){
	address it;
	int k = 0;
	if (L == NULL) return;
	it = L->first;
	while (it!=L->last) {
		k++;
		print_point(it->p);
		printf(" ");
		it = it->next_element;
	}
	print_point(it->p);
}

void draw_polygon(polygon* L){
    element_polygon* it = L->first, *prev = NULL;
    while (it != NULL) {
        if (it->p->status == POINT_STATUS_PENDOWN) {
            setLine(prev->p->x, prev->p->y,it->p->x, it->p->y, 15);
        } else if (it->p->status == POINT_STATUS_PENUP) {
        }
        prev = it;
        it = it->next_element;
    }
}
void copy_polygon(polygon* dest, polygon* src){
    element_polygon* it = src->first;
    while (it != NULL) {
        insert_point(dest, ctor_point(it->p->x,it->p->y,it->p->status));
        it = it->next_element;
    }
}

void rotate_polygon(polygon* p, double radian){
    
}
void translate_polygon(polygon* p, int x, int y){
    address it = p->first;
    while (it!= NULL){
        translate_point(it->p, x, y);
        it = it->next_element;
    }
}
void scale_polygon(polygon* p, int x, int y){
    address it = p->first;
    while (it!= NULL){
        scale_point(it->p, x, y);
        it = it->next_element;
    }
}