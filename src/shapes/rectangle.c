#include <stdio.h>

#include "pel.h"

#include "handle.h"
#include "image.h"
#include "pixel.h"

pel_cord_t rectangle_start;
pel_cord_t rectangle_end;

int pel_draw_rectangle(pel_color_t brush_color, pel_cord_t start, pel_cord_t end)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    _set_color(brush_color);

    // Horizontals
    if (pel_draw_line(brush_color, start, PEL_CORD(end._x, start._y)) ||
        pel_draw_line(brush_color, PEL_CORD(start._x, end._y), end) ||
        // Verticals
        pel_draw_line(brush_color, start, PEL_CORD(start._x, end._y)) ||
        pel_draw_line(brush_color, end, PEL_CORD(end._x, start._y)))
    {
        return -1;
    }

    return 0;
}

// This function should be called specifying a rect since it will color (x,y) every tiem when called
void rectangle_full_draw_cb(int x, int y, pel_color_t px)
{
    _px_set_def_color(x, y);
}

int pel_draw_rectangle_full(pel_color_t brush_color, pel_cord_t start, pel_cord_t end)
{
    rectangle_start = start;
    rectangle_end = end;

    _set_color(brush_color);

    if (pel_draw_rectangle(brush_color, start, end)) return -1;

    return _image_draw_rect(rectangle_full_draw_cb, start, end);
}