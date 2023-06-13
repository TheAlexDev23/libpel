#ifndef _PEL_JPEG_EASY_H_INCLUDED
#define _PEL_JPEG_EASY_H_INCLUDED

#include <stdint.h>

#include "pel.h"

typedef struct _jpeg_easy_jpeg {
    int width;
    int height;

    int subsamp;

    uint8_t* pixels;
} jpeg_easy_jpeg_t;

#define PEL_JPEG_EASY_QUALITY 90

/* Creates empty default jpeg */
int _jpeg_easy_create_empty(char* filename, int width, int height);

/* Reades filename data into jpeg */
int _jpeg_easy_read(char* filename, jpeg_easy_jpeg_t* jpeg);

/* Writes jpeg data into filename */
int _jpeg_easy_write(char* filename, jpeg_easy_jpeg_t jpeg);

/* Callback used for _jpeg_easy_draw */
typedef void (*_jpeg_easy_draw_cb)(int x, int y, uint8_t* px);

/* Calls draw_cb for each pixel betwen rect_start and rect_end */
int _jpeg_easy_draw(jpeg_easy_jpeg_t jpeg, _jpeg_easy_draw_cb draw_cb, pel_cord_t rect_start, pel_cord_t rect_end);

/* Returns pointer to pixel at (x,y) */
uint8_t* _jpeg_easy_px(jpeg_easy_jpeg_t jpeg, int x, int y);

#endif