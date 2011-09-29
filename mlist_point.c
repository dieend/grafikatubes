#include "lpoint.h"
#include "drawing.h"
int main() {
	polygon* L = ctor_polygon(), *X;
        start_mode_vga();
        clearBuffer(double_buffer);
	insert_point(L,ctor_point(10,10,1));
	insert_point(L,ctor_point(10,5,0));
	insert_point(L,ctor_point(15,5,0));
	insert_point(L,ctor_point(15,10,0));
	insert_point(L,ctor_point(15,8,1));
	insert_point(L,ctor_point(10,8,0));
/*
	print_list_point(L);
*/
        copy_polygon(X,L);
        draw_polygon(X);
        showBuffer(double_buffer);
        getch();
        draw_polygon(L);
        showBuffer(double_buffer);
        getch();
	dtor_list_point(X);
        dtor_list_point(L);
        showBuffer(double_buffer);
        getch();
        exit_mode_vga();
        return 0;
}