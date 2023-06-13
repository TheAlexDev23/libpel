#ifndef _PEL_IMAGE_H_INCLDUED
#define _PEL_IMAGE_H_INCLDUED

#include <png.h>

#include "pel.h"

typedef struct _pel_image {
    /* Pointer to image structure according to the type of image. Either png_easy_png_t or ... */
    void* image_structure;
    pel_color_t** pixels;
} pel_image_t;

/* Makes x,y = center + x,y on an image grid (starts top left) */
#define get_xy_rel_img_center(x, y, handle) x = handle->_centerX + x; y = handle->_centerY - y;

/* Makes x,y = center + x,y on an mathematical grid  */
#define get_xy_rel_grid_center(x, y, handle) (x = x - handle->_centerX) * -1; y = (y - handle->_centerY) * -1;

/* Callback type for drawing */
typedef void (*_pel_image_draw_cb_t)(int x, int y, pel_color_t px);

/* Create empty image */
int _image_create_empty(char* filename, pel_image_source_type_t image_type, int width, int height);

/* Load image data into current handle */
int _image_read();

/* Iterates through all pixels of current image calling draw_cb for each */
int _image_draw(_pel_image_draw_cb_t draw_cb);

/* Draws an image in the bonds of rect_start, rect_end */
int _image_draw_rect(_pel_image_draw_cb_t draw_cb, pel_cord_t rect_start, pel_cord_t rect_end);

/* Saves image data from handle into filename specified in handle */
int _image_write();

/* Set a pixel with default color */
int _px_set_def_color(int x, int y);

/* Set a pixel specifiying a color */
int _px_set(int x, int y, pel_color_t color);

#endif
