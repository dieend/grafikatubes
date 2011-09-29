#include "lpoint.h"

int main() {
        point* p;
/*
        start_mode_vga();
        clearBuffer(double_buffer);
*/
        p = ctor_point(10,11,0);
        draw_point(p);
        draw_point(ctor_point(10,12,0));
        
	print_point(p);
	printf("\n");
	rotate_point(p, radian_from_degree(180));
	print_point(p);
	printf("\n");
	translate_point(p,2,-3);
	print_point(p);
	printf("\n");
	scale_point(p,3,4);
	print_point(p);
	printf("\n");
	dtor_point(p);
        
/*
        showBuffer(double_buffer);
*/
        getch();
/*
	exit_mode_vga();
*/
        
        return 0;
}