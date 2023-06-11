#include <png.h>

#include "pel.h"

#include "helper/png-easy.h"
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

    _set_color(brush_color);

    // Change drawing direction if going towards left on the x axis
    if (start._x > end._x)
    {
        pel_cord_t temp = start;
        start = end;
        end = temp;
    }

    /* 
    * This algorithm is known as Bresenham's Line algorithm
    * It only uses integer opeartions which makes it a fast one
    * However, it doesn't offer any anti alising resulting in pixel perfect lines
    */

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

        int rx = x, ry = y;
        get_xy_rel_img_center(rx, ry, handle);
        _px_set_def_color(rx, ry);
    }
    
    return 0;
}