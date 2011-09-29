#include "font.h"
#include <stdio.h>
int main() {
    FONT* F;
	char c;
    init_font_db("data.txt");
	c = 'E';
    print_font(basic_font[c]);
    getch();
    start_mode_vga();
    clearBuffer(double_buffer);
    F = set_font(c,0,0);
    draw_font(F);
    translate_font(F, 35,65);
    draw_font(F);
    scale_font(F, 2,2);
    draw_font(F);
    dtor_font(F);
/*
    F = set_font('E',25,10);
    draw_font(F);
    dtor_font(F);
    F = set_font('F',40,10);
    draw_font(F);
    dtor_font(F);
    F = set_font('I',65,10);
    draw_font(F);
    dtor_font(F);
*/
    showBuffer(double_buffer);
    getch();
    exit_mode_vga();
    clear_font_db();

    return 0;
 }