#ifndef _PEL_PIXEL_H_INCLUDED
#define _PEL_PIXEL_H_INCLUDED

#include <png.h>

#include "pel.h"

void _set_color(pel_color_t color);
pel_color_t _get_color();

void _png_px_set(png_bytep px, pel_color_t color);
void _png_px_set_def_color(png_bytep px);
pel_color_t png_px_to_pel_color(png_bytep px);

#endif