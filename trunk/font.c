#include "font.h"

FONT* basic_font[256];

FONT* ctor_font(char type, int x, int y){
	FONT* font = (FONT*) malloc(sizeof(FONT));
	font->type = type;
	font->height = FONT_HEIGHT;
	font->x = x;
	font->y = y;
	font->points = ctor_polygon();
	return font;
}

void dtor_font(FONT* font){
	dtor_list_point(font->points);
	free(font);
}
void print_font(FONT* font) {
	printf("character %c:\n",font->type);
	printf("position at (%d,%d)",font->x,font->y);
	printf("\n");
	print_list_point(font->points);
}
FONT* set_font(char type, int x, int y){
    FONT* f = ctor_font(type,x,y);
    f->height = basic_font[type]->height;
    f->width = basic_font[type]->width;
    copy_polygon(f->points, basic_font[type]->points);
    return f;
}
void rotate_font(FONT* font, double radian){
}
void translate_font(FONT* font, int x, int y){
    translate_polygon(font->points, x, y);
}
void scale_font(FONT* font, int x, int y){
    scale_polygon(font->points, x, y, 0, 14);
}
void draw_font(FONT* font){
    element_polygon* it = font->points->first, *prev = NULL;
    while (it != NULL) {
        if (it->p->status == POINT_STATUS_PENDOWN) {
            setLine(font->x+prev->p->x, font->y+prev->p->y-15,font->x+it->p->x, font->y+it->p->y-15, 15);
        } else if (it->p->status == POINT_STATUS_PENUP) {
        }
        prev = it;
        it = it->next_element;
    }
}
void clear_font_db() {
    int i = 0;
    for (i=0; i<256; i++){
        if (basic_font[i] != NULL) dtor_font(basic_font[i]);
    }
}
void init_font_db(char* file_name) {
    char f;
    int x,y,status,h,w;
    FILE * myfile;
    h=0; w=0;
    
/*
    freopen(file_name,"r",stdin);
*/
    
    myfile = fopen(file_name,"r");
    
    while (fscanf(myfile,"%c\n",&f) == 1){
        basic_font[f] = ctor_font(f,0,0);
        fscanf(myfile,"%d %d %d", &x,&y,&status);
        while (status!=-1){
            if (h > y) h = y;
            if (w < x) w = x;
            insert_point(basic_font[f]->points,ctor_point(x,y,status));
            fscanf(myfile,"%d %d %d", &x,&y,&status);
        }
        basic_font[f]->height = h<0?-h:h;
        basic_font[f]->width = w<0?-w:w;
    }
    fclose(myfile);
}