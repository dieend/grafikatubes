#include "font.h"
#include <stdio.h>
void show(char c, int x, int y) {
	FONT *F;
	
	F = set_font(c,x,y);
    draw_font(F);
    translate_font(F, 16,0);
    scale_font(F, 3,3);
    draw_font(F);
    dtor_font(F);
}
int main() {
    FONT* F;
	char c;
    init_font_db("data.txt");
	c = 'A';
    /*print_font(basic_font[c]);*/
    start_mode_vga();
    clearBuffer(double_buffer);
	show('K',10,30);
	show('N',10,80);
	show('O',10,130);
	show('I',10,180);
	/*
    F = set_font(c,80,80);
    draw_font(F);
    translate_font(F, 35,65);
    scale_font(F, 3,3);
    draw_font(F);
    dtor_font(F);
	*/
	
    showBuffer(double_buffer);
    getch();
    exit_mode_vga();
    clear_font_db();

    return 0;
 }