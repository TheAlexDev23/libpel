#include "pel.h"
#include "image.h"

void graysacle_draw_cb(int x, int y, pel_color_t px)
{
    int color_avg = (px.r + px.g + px.b) / 3;

    pel_color_t new_color;
    new_color.r = color_avg;
    new_color.g = color_avg;
    new_color.b = color_avg;
    new_color.a = px.a;

    _px_set(x, y, new_color);
}

int pel_filter_graysacle()
{
    return _image_draw(graysacle_draw_cb);
}