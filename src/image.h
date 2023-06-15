#ifndef _PEL_IMAGE_H_INCLDUED
#define _PEL_IMAGE_H_INCLDUED

#include <png.h>
#include <stdbool.h>

#include "pel.h"

/* Makes x,y = center + x,y on an image grid (starts top left) */
#define get_xy_rel_img_center(x, y, handle) x = handle->_centerX + x; y = handle->_centerY - y;

/* Makes x,y = center + x,y on an mathematical grid  */
#define get_xy_rel_grid_center(x, y, handle) (x = x - handle->_centerX) * -1; y = (y - handle->_centerY) * -1;

/* Returns the image type according to the extension of the file fn */
pel_image_type _image_type(char* fn);

int _image_dimensions(char* fn, int* width, int* height);

/* Create empty image */
int _image_create_empty(char* filename, pel_image_type image_type, int width, int height);

/* Load image data into current handle. If in true will save data into in image structure */
int _image_read(bool in);

/* Callback type for drawing */
typedef void (*_pel_image_draw_cb_t)(int x, int y, pel_color_t px);

/* Iterates through all pixels of current image calling draw_cb for each.  */
int _image_draw(_pel_image_draw_cb_t draw_cb);

/* Draws an image in the bonds of rect_start, rect_end */
int _image_draw_rect(_pel_image_draw_cb_t draw_cb, pel_cord_t rect_start, pel_cord_t rect_end);

/* Saves image data from handle into filename specified in handle. */
int _image_write();

/* Set a pixel with default color */
int _px_set_def_color(int x, int y);

/* Set a pixel specifiying a color */
int _px_set(int x, int y, pel_color_t color);

#endif
