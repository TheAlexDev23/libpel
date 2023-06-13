#include <stdlib.h>
#include <math.h>

#include "pel.h"

#include "state.h"
#include "image.h"
#include "pixel.h"

int pel_draw_triangle(pel_color_t brush_color, pel_cord_t p1, pel_cord_t p2, pel_cord_t p3)
{
    CHECK

    if (pel_draw_line(brush_color, p1, p2) ||
        pel_draw_line(brush_color, p1, p3) ||
        pel_draw_line(brush_color, p2, p3))
    {
        return -1;
    }

    return 0;
}

// Reference: https://www.mathopenref.com/coordtrianglearea.html
/* Returns area of triangle p1p2p3 */
float triangle_area(pel_cord_t p1, pel_cord_t p2, pel_cord_t p3)
{
    return abs((float)(p1._x * (p2._y - p3._y) + p2._x * (p3._y - p1._y) + p3._x * (p1._y - p2._y)) / 2.0);
}

/* Triangle fill points */
pel_cord_t tf_p1;
pel_cord_t tf_p2;
pel_cord_t tf_p3;

/* Precalculated value of tf triangle area */
float tf_a;
#define ALLOWED_DIFF 1
void triangle_full_draw_cb(int x, int y, pel_color_t px)
{
    pel_handle_t* handle;
    if ((handle = _pel_get_cur_handle()) == NULL) return;

    int rx = x, ry = y;

    get_xy_rel_grid_center(rx, ry, handle);

    // If a point pt is inside triangle ABC
    // areas of ptAB + ptAC + ptBC should be equal to area of ABC
    pel_cord_t pt = PEL_CORD(rx, ry);

    float a1 = triangle_area(pt, tf_p1, tf_p2);
    float a2 = triangle_area(pt, tf_p1, tf_p3);
    float a3 = triangle_area(pt, tf_p2, tf_p3);

    if (a1 + a2 + a3 - tf_a <= ALLOWED_DIFF)
        _px_set_def_color(x, y);
}

int pel_draw_triangle_full(pel_color_t brush_color, pel_cord_t p1, pel_cord_t p2, pel_cord_t p3)
{
    CHECK

    pel_handle_t* handle = _pel_get_cur_handle();
    
    if (pel_draw_triangle(brush_color, p1, p2, p3)) return -1;

    tf_p1 = p1;
    tf_p2 = p2;
    tf_p3 = p3;

    tf_a = triangle_area(p1, p2, p3);

    _set_color(brush_color);

    int minx = fmin(fmin(p1._x, p2._x), p3._x); 
    int miny = fmin(fmin(p1._y, p2._y), p3._y); 
    int maxx = fmax(fmax(p1._x, p2._x), p3._x); 
    int maxy = fmax(fmax(p1._y, p2._y), p3._y);

    pel_cord_t start = PEL_CORD(minx, miny);
    pel_cord_t end = PEL_CORD(maxx, maxy);

    return _image_draw_rect(triangle_full_draw_cb, start, end);
}