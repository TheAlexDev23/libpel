#ifndef _PEL_INCLUDED
#define _PEL_INCLUDED

#include "error.h"

typedef enum _pel_image_type {
    PEL_IMG_PNG,
    PEL_IMG_JPEG,
    PEL_IMG_WEBP,
    PEL_IMG_CORRUPT
} pel_image_type;

typedef enum {
    PEL_PNG_FORMAT_RGBA,
    PEL_PNG_FORMAT_RGB
} pel_png_format;

/********************************
 * 
 *  Colors
 * 
**********************************/

/* Pel works with RGBA values even with RGB or other format images */
typedef struct _pel_color {
    int r;
    int g;
    int b;
    int a;
} pel_color_t;

#define PEL_COLOR_BLACK (pel_color_t){.r = 0, .g = 0, .b = 0, .a = 255}
#define PEL_COLOR_WHITE (pel_color_t){.r = 255, .g = 255, .b = 255, .a = 255}
#define PEL_COLOR_RED (pel_color_t){.r = 255, .g = 0, .b = 0, .a = 255}
#define PEL_COLOR_GREEN (pel_color_t){.r = 0, .g = 255, .b = 0, .a = 255}
#define PEL_COLOR_BLUE (pel_color_t){.r = 0, .g = 0, .b = 255, .a = 255}

/********************************
 * 
 *  Pixels
 * 
**********************************/

typedef struct _pel_bitmap {
    int height;
    int width;
    pel_color_t* bm;
} pel_bitmap_t;

typedef struct _pel_cord {
    int _x;
    int _y;
} pel_cord_t;

/* Facilitates the creatin of pel_cord_t */
#define PEL_CORD(x, y) (pel_cord_t){._x = x, ._y = y}

/********************************
 * 
 *  Text
 * 
**********************************/

typedef struct _pel_font {
    char* font_family;
    char* font_style;
    int size;
} pel_font_t;

/* Facilitates the creation of pel_font_t */
#define PEL_FONT(family, style, sz) (pel_font_t){.font_family = family, .font_style = style, .size = sz}

typedef enum _pel_text_align {
    PEL_TEXT_ALIGN_CENTER,
    PEL_TEXT_ALIGN_START, 
    PEL_TEXT_ALIGN_END
} pel_text_align_t;

/********************************
 * 
 *  Init/Exit
 * 
**********************************/

/* 
* Sets the source and destination files 
* @param fn_src Filename of 'in' file
* @param fn_dst Filename of 'out' file
*/
extern int pel_set_src_dest(char* fn_src, char* fn_dst);

/* Initialazes pel */
extern int pel_init(int width, int height);

/* Closes pel and saves modifications */
extern int pel_save();


/********************************
 * 
 *  Configuration
 * 
**********************************/

/* 
* Modifies the default configuration values when creating PNGs. 
* @param color_type should be pel_png_format
*/
extern int pel_conf_png(int bit_depth, int color_type);
/* Modifies the default configuration values when creating JPEGs. */
extern int pel_conf_jpeg(int quality);
/* Modifies the default configuration values when creating WebPs. */
extern int pel_conf_webp();


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
* Draws a rectangle from point start to point end
*/
extern int pel_draw_rectangle(pel_color_t brush_color, pel_cord_t start, pel_cord_t end);

/*
* Drwas a filled rectange from point start to point end
*/
extern int pel_draw_rectangle_full(pel_color_t brush_color, pel_cord_t start, pel_cord_t end);

/*
* Draws line from point start to point end
*/
extern int pel_draw_line(pel_color_t brush_color, pel_cord_t start, pel_cord_t end);

/*
* Writes a textbox
* @param box_width Upon reaching this width will write on next line
* @param line_offset Offset when going on to next line. Font height isn't taken into account when computing offset so line_offset is the only factor that affects line separation
* @param cords coordinates relative center for text placement.
*/
extern int pel_draw_textbox(pel_color_t brush_color, pel_font_t font, int box_width, int line_offset, pel_text_align_t align, char* text, pel_cord_t cords);

/*
* Draws triangle P1P2P3
*/
extern int pel_draw_triangle(pel_color_t brush_color, pel_cord_t p1, pel_cord_t p2, pel_cord_t p3);

/*
* Draws filled triangle P1P2P3
*/
extern int pel_draw_triangle_full(pel_color_t brush_color, pel_cord_t p1, pel_cord_t p2, pel_cord_t p3);

#endif
