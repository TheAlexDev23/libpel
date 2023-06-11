#include <png.h>

#include "pel.h"

#include "image.h"
#include "pixel.h"
#include "handle.h"
#include "helper/png-easy.h"
#include "state.h"

int circle_r;
int circle_x;
int circle_y;

void draw_circle_cb(int x, int y, pel_color_t px)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return;

    int rx = x, ry = y;
    get_xy_rel_grid_center(rx, ry, handle);

    // (y - yoffset)² = r² - (x - xoffset)²
    if (((ry - circle_y) * (ry - circle_y)) -
        (circle_r * circle_r - (rx - circle_x) * (rx - circle_x)) < circle_r &&
        ((ry - circle_y) * (ry - circle_y)) -
        (circle_r * circle_r - (rx - circle_x) * (rx - circle_x)) > -1 * circle_r)
    {
        _px_set_def_color(x, y);
    }
}

void draw_circle_full_cb(int x, int y, pel_color_t px)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return;

    int rx = x, ry = y;
    get_xy_rel_grid_center(rx, ry, handle);
    // (y - yoffset)² = r² - (x - xoffset)²
    if ((ry - circle_y) * (ry - circle_y) -
        (circle_r * circle_r - (rx - circle_x) * (rx - circle_x)) < circle_r)
    {
        _px_set_def_color(x, y);
    }
}

int draw_circle_base(pel_color_t brush_color, int x, int y, int r, _pel_image_draw_cb_t draw_cb)
{
    pel_handle_t* handle;
    if ((handle = _pel_get_cur_handle()) == NULL) return -1;

    png_easy_png_t png;

    circle_r = r;
    circle_x = x;
    circle_y = y;

    _set_color(brush_color);

    if (_image_draw(draw_cb))
    {
        handle->_err = PEL_ERR_PNG_EASY;
        return -1;
    }

    return 0;
}

int pel_draw_circle(pel_color_t brush_color, pel_cord_t cords, int r)
{
    CHECK
    return draw_circle_base(brush_color, cords._x, cords._y, r, draw_circle_cb);
}

int pel_draw_circle_full(pel_color_t brush_color, pel_cord_t cords, int r)
{
    CHECK
    return draw_circle_base(brush_color, cords._x, cords._y, r, draw_circle_full_cb);
}