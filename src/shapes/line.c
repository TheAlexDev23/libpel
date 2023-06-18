#include <stdlib.h>
#include <math.h>
#include <png.h>

#include "pel.h"
#include "line.h"

#include "helper/png-easy.h"
#include "state.h"
#include "pixel.h"
#include "image.h"

void line_eq(pel_cord_t start, pel_cord_t end, float* m, float* b)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return;

    int dx, dy;

    get_line_dxdy(start, end, &dx, &dy);

   *m = 0;

    if (dx != 0)
        *m = (float)dy / (float)dx;
    else
        // This value should techinically be infinity. But since we can't really to that we increase the slope proportionally to screen height
        *m = handle->_height * 2;

    // y = mx + b
    // b = y1 - mx1
    *b = start._y - (*m) * start._x;
}

void get_line_dxdy(pel_cord_t start, pel_cord_t end, int* dx, int *dy)
{
    *dx = end._x - start._x, *dy = end._y - start._y;
}

int pel_draw_line(pel_color_t brush_color, pel_cord_t start, pel_cord_t end)
{
    CHECK

    pel_handle_t* handle = _pel_get_cur_handle();
    if (end._x >= handle->_width ||
        end._y >= handle->_height)
        return -1;

    if (start._x > end._x) {
        pel_cord_t temp = start;
        start = end;
        end = temp;
    }

    if (start._y > end._y) {
        pel_cord_t temp = start;
        start = end;
        end = temp;
    }

    _set_color(brush_color);

    int dx, dy;
    get_line_dxdy(start, end, &dx, &dy);

    int ix = dx < 0 ? -1 : 1;
    int iy = dy < 0 ? -1 : 1;

    float m, b;
    line_eq(start, end, &m, &b);

    if (dx > iy * dy) {
        int ly = m * start._x + b;
        for (int x = start._x; x <= end._x; x += ix) {
            float y = m*(float)x + b;

            int rx = x, ry = y;

            get_xy_rel_img_center(rx, ry, handle);
            _px_set_def_color(rx, ry);

            int lydiff = abs(ly - (int)y);

            if (lydiff > 1) {
                for (int i = (ly - 1) * iy; i < iy * y; i++) {
                    int rx = x, ry = i * iy;
                    get_xy_rel_img_center(rx, ry, handle);
                    _px_set_def_color(rx, ry);
                }
            }

            ly = y;
        }
    }
    else {

        int lx = (start._y - b)/m;
        for (int y = start._y; y <= end._y; y += iy) {
            // y = mx + b
            // x = (y - b) / m
            float x = ((float)y - b) / m;

            int rx = x, ry = y;

            get_xy_rel_img_center(rx, ry, handle);
            _px_set_def_color(rx, ry);

            int lxdiff = abs(lx - (int)x);

            if (lxdiff > 1) {
                for (int i = (lx - 1) * ix; i < ix * x; i++) {
                    int rx = i * ix, ry = y;
                    get_xy_rel_img_center(rx, ry, handle);
                    _px_set_def_color(rx, ry);
                }
            }

            lx = x;
        }
    }

    return 0;
}
