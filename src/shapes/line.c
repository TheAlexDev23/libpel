#include <png.h>

#include "pel.h"

#include "png-easy.h"
#include "state.h"
#include "pixel.h"
#include "image.h"

int pel_draw_line(pel_color_t brush_color, pel_cord_t start, pel_cord_t end)
{
    CHECK

    pel_handle_t* handle = _pel_get_cur_handle();
    if (end._x >= handle->_width ||
        end._y >= handle->_height)
        return -1;

    png_easy_png_t png;

    _set_color(brush_color);

    if (_png_easy_read(handle->_fn, &png)) return -1;

    int dx = end._x - start._x, dy = end._y - start._y;
    int ix = 0, iy = 0;
    if (dx > 0) ix = 1;
    if (dx < 0) ix = -1;
    if (dy > 0) iy = 1;
    if (dy < 0) iy = -1;

    int err = 0;

    for (int x = start._x, y = start._y; x < end._x; x += ix)
    {
        err += dy * iy;
        if (err >= dx * ix)
        {
            y += iy;
            err -= dx;
        }

        _px_set_def_color(_png_easy_px(png, x, y));
    }
    
    if (_png_easy_write(handle->_fn, png)) return -1;
    
    return 0;
}