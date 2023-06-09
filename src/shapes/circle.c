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

void draw_circle_cb(int x, int y, png_bytep px)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return;

    get_xy_rel_grid_center(x, y, handle);

    // (y - yoffset)² = r² - (x - xoffset)²
    if (((y - circle_y) * (y - circle_y)) -
        (circle_r * circle_r - (x - circle_x) * (x - circle_x)) < circle_r &&
        ((y - circle_y) * (y - circle_y)) -
        (circle_r * circle_r - (x - circle_x) * (x - circle_x)) > -1 * circle_r)
    {
        _px_set_def_color(px);
    }
}

void draw_circle_full_cb(int x, int y, png_bytep px)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return;

    get_xy_rel_grid_center(x, y, handle);
    // (y - yoffset)² = r² - (x - xoffset)²
    if ((y - circle_y) * (y - circle_y) -
        (circle_r * circle_r - (x - circle_x) * (x - circle_x)) < circle_r)
    {
        _px_set_def_color(px);
    }
}

int draw_circle_base(pel_color_t brush_color, int x, int y, int r, _png_easy_draw_cb draw_cb)
{
    pel_handle_t* handle;
    if ((handle = _pel_get_cur_handle()) == NULL) return -1;

    png_easy_png_t png;

    circle_r = r;
    circle_x = x;
    circle_y = y;

    _set_color(brush_color);

    if (_png_easy_read(handle->_fn, &png) ||
        _png_easy_draw(png, draw_cb) ||
        _png_easy_write(handle->_fn, png))
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