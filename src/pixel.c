#include <stddef.h>

#include "pixel.h"
#include "handle.h"

pel_color_t _cur_color;

void _set_color(pel_color_t color) { _cur_color = color; }

pel_color_t _get_color() { return _cur_color; }

void _png_px_set_def_color(png_bytep px)
{
    if (px == NULL) return;

    px[0] = _cur_color.r;
    px[1] = _cur_color.g;
    px[2] = _cur_color.b;
    px[3] = _cur_color.a;
}

void _png_px_set(png_bytep px, pel_color_t color)
{
    if (px == NULL) return;

    _set_color(color);
    _png_px_set_def_color(px);
}

/* Converts a png pixel into a pel color */
pel_color_t png_px_to_pel_color(png_bytep px)
{
    pel_color_t pel_color;
    pel_color.r = px[0];
    pel_color.g = px[1];
    pel_color.b = px[2];
    pel_color.a = px[3];

    return pel_color;
}

int _px_set_def_color(int x, int y)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    /* Return 0 since in most cases we ignore height and width */
    if (x >= handle->_width || y >= handle->_height || x < 0 || y < 0) return 0;

    handle->_img.pixels[y][x] = _cur_color;

    return 0;
}

int _px_set(int x, int y, pel_color_t color)
{
    _set_color(color);
    return _px_set_def_color(x, y);
}