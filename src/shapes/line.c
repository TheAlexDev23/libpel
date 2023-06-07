#include <png.h>

#include "pel.h"

#include "png-easy.h"
#include "state.h"
#include "pixel.h"

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

    // Absolutely no idea how this works. It is the Bresenham's line algorithm

    float m = (float)(end._y - start._y) / (float)(end._x - start._x);

    printf("1\n");

    int err = 0;
    int y = start._y;
    for (int x = start._x; x < end._x; x++)
    {
        png_bytep px = _png_easy_px(png, x, y);
        if (px == NULL) return -1;

        _px_set_def_color(px);

        if ((float)err + m < 0.5)
        {
            err += m;
        }
        else
        {
            y++;
            err += m - 1;
        }
    }
    
    if (_png_easy_write(handle->_fn, png)) return -1;
    
    return 0;
}