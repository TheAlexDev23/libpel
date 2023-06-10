#include <stddef.h>

#include "pixel.h"

pel_color_t _cur_color;

void _set_color(pel_color_t color)
{
    _cur_color = color;
}

void _px_set_def_color(png_bytep px)
{
    px[0] = _cur_color.r;
    px[1] = _cur_color.g;
    px[2] = _cur_color.b;
    px[3] = _cur_color.a;
}

void _px_set(png_bytep px, pel_color_t color)
{
    if (px == NULL)
    {
        return;
    }

    _set_color(color);
    _px_set_def_color(px);
}