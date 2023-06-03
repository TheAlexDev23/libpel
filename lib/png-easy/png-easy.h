#ifndef _PNG_EASY_INCLUDED
#define _PNG_EASY_INCLUDED

#include <png.h>
typedef struct _png_easy_png {
    int width;
    int height;

    png_byte color_type;
    png_byte bit_depth;
    png_bytep* row_pointers;
} png_easy_png_t;

/* Reads file filename writes to png_easy if succesful */
int png_easy_read(char* filename, png_easy_png_t* png_easy);

int png_easy_create_empty(char* filename, int width, int height);

/* Writes row_pointers into filename if succesful */
int png_easy_write(char* filename, png_bytep* row_pointers, int width, int height);

/* Draws png by calling draw_cb for each pixel */
int png_easy_draw(png_easy_png_t* png, void (*draw_cb(int x, int y, png_bytep px)));

#endif