#ifndef _PEL_PIXEL_H_INCLUDED
#define _PEL_PIXEL_H_INCLUDED

#include <png.h>

#include "pel.h"

void _set_color(pel_color_t color);
/* Set pixel with current color */
void _px_set_def_color(png_bytep px);
void _px_set(png_bytep px, pel_color_t color);

#endif