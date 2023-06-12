#include <stdlib.h>
#include <math.h>
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

    if (start._x > end._x)
    {
        pel_cord_t temp = start;
        start = end;
        end = temp;
    }

    if (start._y > end._y)
    {
        pel_cord_t temp = start;
        start = end;
        end = temp;
    }

    int dx = end._x - start._x, dy = end._y - start._y;

    float m = 0;

    if (dx != 0)
        m = (float)dy / (float)dx;
    else
        // This value should techinically be infinity. But since we can't really to that we increase the slope proporsionally to screen height
        m = handle->_height;

    // y = mx + b
    // b = y1 - mx1
    float b = start._y - m * start._x;

    int ix = dx < 0 ? -1 : 1;
    int iy = dy < 0 ? -1 : 1;

    if (dx > iy * dy)
    {
        for (int x = start._x; x < end._x; x += ix)
        {
            float y = m*(float)x + b;

            int rx = x, ry = (int)round(y);

            get_xy_rel_img_center(rx, ry, handle);
            _px_set_def_color(rx, ry);
        }
    }
    else
    {
        for (int y = start._y; y < end._y; y += iy)
        {
            // y = mx + b
            // x = (y - b) / m
            float x = ((float)y - b) / m;

            int rx = (int)round(x), ry = y;

            get_xy_rel_img_center(rx, ry, handle);
            _px_set_def_color(rx, ry);
        }
    }

    return 0;
}