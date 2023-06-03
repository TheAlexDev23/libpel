#include <png.h>

#include "pel.h"
#include "png-easy.h"

int img_center_x;
int img_center_y;

pel_color_t color;

int pel_init(char* filename, int height, int width)
{
    img_center_x = width / 2;
    img_center_y = height / 2;
    return png_easy_create_empty(filename, height, width);
}

int circle_r;
int circle_x;
int circle_y;

void draw_circle_cb(int x, int y, png_bytep px)
{
    printf("%i %i %i %i\n", color.r, color.g, color.b, color.a);
    x = img_center_x - x;

    y = img_center_y - y;

    // (y - yoffset)² = r² - (x - xoffset)²
    if (((y - circle_y) * (y - circle_y)) -
        (circle_r * circle_r - (x - circle_x) * (x - circle_x)) < circle_r &&
        ((y - circle_y) * (y - circle_y)) -
        (circle_r * circle_r - (x - circle_x) * (x - circle_x)) > -1 * circle_r)
    {
        px[0] = color.r;
        px[1] = color.g;
        px[2] = color.b;
        px[3] = color.a;
    }
}

int pel_draw_circle(char* filename, pel_color_t brush_color, int x, int y, int r)
{
    png_easy_png_t png;

    circle_r = r;
    circle_x = x;
    circle_y = y;

    color = brush_color;

    if (png_easy_read(filename, &png) ||
        png_easy_draw(png, draw_circle_cb) ||
        png_easy_write(filename, png))
    {
        return -1;
    }

    return 0;
}