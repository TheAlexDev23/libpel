#ifndef _PEL_INCLUDED
#define _PEL_INCLUDED

#include "error.h"

typedef struct _pel_color {
    int r;
    int g;
    int b;
    int a;
} pel_color_t;

typedef struct _pel_px {
    int x;
    int y;
    pel_color_t color;
} pel_px_t;

typedef struct _pel_bitmap {
    int height;
    int width;
    pel_color_t* bm;
} pel_bitmap_t;

#define PEL_COLOR_BLACK (pel_color_t){.r = 0, .g = 0, .b = 0, .a = 255}
#define PEL_COLOR_WHITE (pel_color_t){.r = 255, .g = 255, .b = 255, .a = 255}
#define PEL_COLOR_RED (pel_color_t){.r = 255, .g = 0, .b = 0, .a = 255}
#define PEL_COLOR_GREEN (pel_color_t){.r = 0, .g = 255, .b = 0, .a = 255}
#define PEL_COLOR_BLUE (pel_color_t){.r = 0, .g = 0, .b = 255, .a = 255}

typedef struct _pel_cord {
    int _x;
    int _y;
} pel_cord_t;

#define PEL_CORD(x, y) (pel_cord_t){._x = x, ._y = y}

typedef struct _pel_font {
    char* font_family;
    char* font_style;
    int size;
} pel_font_t;

#define PEL_FONT(family, style, sz) (pel_font_t){.font_family = family, .font_style = style, .size = sz}

/* Initializes PEL file */
extern int pel_init(char* filename, int height, int width);

/********************************
 * 
 *  Shapes
 * 
**********************************/

/* 
* Draws circle of radius r with it's center at image center + (cords)
* Filename needs to be valid png otherwise will return -1
*/
extern int pel_draw_circle(pel_color_t brush_color, pel_cord_t cords, int r);

/* 
* Draws filled circle of radius r with it's center at image center + (cords)
* Filename needs to be valid png otherwise will return -1
*/
extern int pel_draw_circle_full(pel_color_t brush_color, pel_cord_t cords, int r);

/*
* Draws line from point start to point end
*/
extern int pel_draw_line(pel_color_t brush_color, pel_cord_t start, pel_cord_t end);

/*
* Draws text with selected font. With defines the point when should go next line.
*/
extern int pel_write(pel_color_t brush_color, pel_font_t font, int width, int line_offset, char* text, pel_cord_t cords);

#endif