#include <stdlib.h>

#include "helper/png-easy.h"

#include "image.h"
#include "handle.h"
#include "state.h"
#include "pixel.h"

/* Returns png_easy_png_t referenced by the handle->_img.image_structure */
png_easy_png_t png_from_handle(pel_handle_t* handle)
{
    return *((png_easy_png_t*)handle->_img.image_structure);
}

/* Converts a png pixel into a pel color */
pel_color_t png_px_to_pel_color(png_bytep px)
{
    pel_color_t pel_color;
    pel_color.r = px[0];
    pel_color.g = px[1];
    pel_color.b = px[2];
    pel_color.a = px[3];

    return pel_color;
}

/* Converts a png structure into a pel_image_t */
pel_image_t png_to_pel(png_easy_png_t png)
{
    pel_image_t img;
    img.pixels = malloc(png.height * sizeof(pel_color_t*));
    for (int i = 0; i < png.height; i++)
    {
        img.pixels[i] = malloc(png.width * sizeof(pel_color_t));
    }

    for (int j = 0; j < png.height; j++)
    {
        for (int i = 0; i < png.width; i++)
        {
            img.pixels[j][i] = png_px_to_pel_color(
                _png_easy_px(png, i, j));
        }
    }

    return img;
}

int _image_read()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    switch(handle->_image_source_type)
    {
        case PEL_IMG_SOURCE_PNG: ;
            png_easy_png_t* png = malloc(sizeof(png_easy_png_t));
            if (_png_easy_read(handle->_fn, png))
            {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }

            handle->_img = png_to_pel(*png);
            handle->_img.image_structure = png;

            handle->_width = png->width;
            handle->_height = png->height;
            break;
        default:
            handle->_err = PEL_ERR_UNKOWN;
            return -1;
    }

    return 0;
}

int _image_create_empty(char* filename, pel_image_source_type_t image_type, int width, int height)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    switch (image_type)
    {
        case PEL_IMG_SOURCE_PNG: ;
            if (_png_easy_create_empty(filename, width, height))
            {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }
            break;
        default:
            handle->_err = PEL_ERR_UNKOWN;
            return -1;
    }
    return 0;
}

_pel_image_draw_cb_t _cur_draw_cb;

/* This callback converts a png-easy draw callback into a pel draw callback by being used as a callback to _png_easy_draw */
void image_png_draw_cb(int x, int y, png_bytep px)
{
    _cur_draw_cb(x, y, png_px_to_pel_color(px));
}

int _image_draw(_pel_image_draw_cb_t draw_cb)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    pel_cord_t screen_start = PEL_CORD(0, 0);
    pel_cord_t screen_end = PEL_CORD(handle->_width, handle->_height);

    return _image_draw_rect(draw_cb, screen_start, screen_end);
}

int _image_draw_rect(_pel_image_draw_cb_t draw_cb, pel_cord_t rect_start, pel_cord_t rect_end)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    if (rect_start._x > rect_end._x)
    {
        int tmp = rect_start._x;
        rect_start._x = rect_end._x;
        rect_end._x = tmp;
    }
    
    if (rect_start._y > rect_end._y)
    {
        int tmp = rect_start._y;
        rect_start._y = rect_end._y;
        rect_end._y = tmp;
    }

    _cur_draw_cb = draw_cb;

    switch (handle->_image_source_type)
    {
        case PEL_IMG_SOURCE_PNG: ;
            if (_png_easy_draw(png_from_handle(handle), image_png_draw_cb, rect_start, rect_end))
            {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }
            break;
        default:
            handle->_err = PEL_ERR_UNKOWN;
            return -1;
            break;
    }

    return 0;
}

int _image_write()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    switch (handle->_image_source_type)
    {
        case PEL_IMG_SOURCE_PNG: ;
            if (_png_easy_write(handle->_fn, png_from_handle(handle)))
            {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }
            break;
        default:
            handle->_err = PEL_ERR_UNKOWN;
            return -1;
    }

    return 0;
}

int _px_set_def_color(int x, int y)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    switch (handle->_image_source_type)
    {
        case PEL_IMG_SOURCE_PNG: ;
            _png_px_set_def_color(_png_easy_px(png_from_handle(handle), x, y));
            break;
        default:
            handle->_err = PEL_ERR_UNKOWN;
            return -1;
    }

    return 0;
}

int _px_set(int x, int y, pel_color_t color)
{
    _set_color(color);
    return _px_set_def_color(x, y);
}
