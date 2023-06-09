#ifndef _PNG_EASY_INCLUDED
#define _PNG_EASY_INCLUDED

#include <png.h>
#include "pel.h"

typedef struct _png_easy_png {
    int width;
    int height;

    png_byte color_type;
    png_byte bit_depth;
    png_bytep* row_pointers;
} png_easy_png_t;

/* Reads filename image data to png_easy */
int _png_easy_read(char* filename, png_easy_png_t* png);

/* Writes image data into filename if succesful */
int _png_easy_write(char* filename, png_easy_png_t png);

/* Creates empty png with defaulot configuration */
int _png_easy_create_empty(char* filename, int width, int height);

/* Callback used _png_easy_draw */
typedef void (*_png_easy_draw_cb)(int x, int y, png_bytep px);

/* Draws png by calling draw_cb for each pixel */
int _png_easy_draw(png_easy_png_t png, _png_easy_draw_cb draw_cb, pel_cord_t rect_start, pel_cord_t rect_end);

/* Returns pointer to pixel at (x,y) */
png_bytep _png_easy_px(png_easy_png_t png, int x, int y);
/* Sets png pixel with the provided pel color */
void _png_px_set(png_bytep px, pel_color_t color);
/* Sets png pixel with current color */
void _png_px_set_def_color(png_bytep px);
/* Returns pel color with the data in png pixel */
pel_color_t png_px_to_pel_color(png_bytep px);

#endif