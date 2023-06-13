#include <stdlib.h>
#include <string.h>

#include "helper/png-easy.h"
#include "helper/jpeg-easy.h"

#include "image.h"
#include "handle.h"
#include "state.h"
#include "pixel.h"

/* Returns the image type according to the extension of the file fn */
pel_image_type _image_type(char* fn)
{
    char* extension = strstr(fn, ".");
    
    if (strcmp(extension, ".png") == 0) {
        return PEL_IMG_PNG;
    } else if (strcmp(extension, ".jpeg") == 0 || strcmp(extension, ".jpg") == 0) {
        return PEL_IMG_JPEG;
    }

    return PEL_IMG_CORRUPT;
}

/* Returns png_easy_png_t referenced by the handle->_img.image_structure */
png_easy_png_t png_from_handle(pel_handle_t* handle, bool in) 
{ 
    if (in)
        return *((png_easy_png_t*)handle->_in_img.image_structure); 
    else 
        return *((png_easy_png_t*)handle->_out_img.image_structure); 
}

/* Returns jpeg_easy_jpeg_t referenced by the handle->_img.image_structure */
jpeg_easy_jpeg_t jpeg_from_handle(pel_handle_t* handle, bool in) 
{
    if (in)
        return *((jpeg_easy_jpeg_t*)handle->_in_img.image_structure); 
    else
        return *((jpeg_easy_jpeg_t*)handle->_out_img.image_structure); 
}

/* Converts a png structure into a pel_image_t */
pel_image_t png_to_pel(png_easy_png_t png)
{
    pel_image_t img;
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return img;

    handle->pixels = malloc(png.height * sizeof(pel_color_t*));
    for (int i = 0; i < png.height; i++) {
        handle->pixels[i] = malloc(png.width * sizeof(pel_color_t));
    }

    for (int j = 0; j < png.height; j++) {
        for (int i = 0; i < png.width; i++) {
            handle->pixels[j][i] = png_px_to_pel_color(
                _png_easy_px(png, i, j));
        }
    }

    return img;
}

pel_image_t jpeg_to_pel(jpeg_easy_jpeg_t jpeg)
{
    pel_image_t img;
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return img;

    handle->pixels = malloc(jpeg.height * sizeof(pel_color_t*));
    for (int i = 0; i < jpeg.height; i++) {
        handle->pixels[i] = malloc(jpeg.width * sizeof(pel_color_t));
    }

    for (int j = 0; j < jpeg.height; j++) {
        for (int i = 0; i < jpeg.width; i++) {
            handle->pixels[j][i] = jpeg_px_to_pel_color(
                _jpeg_easy_px(jpeg, i, j));
        }
    }

    return img;
}

/* Specifying "if in file" is important because we will eventually read both in and out files */
int _image_read(bool in)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    char* fn = in ? handle->_fn_in : handle->_fn_out;
    pel_image_t* img = in ? &(handle->_in_img) : &(handle->_out_img);

    switch(in ? handle->_image_in_type : handle->_image_out_type) {
        case PEL_IMG_PNG: ;
            png_easy_png_t* png = malloc(sizeof(png_easy_png_t));
            if (_png_easy_read(fn, png)) {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }

            *img = png_to_pel(*png);
            img->image_structure = png;

            handle->_width = png->width;
            handle->_height = png->height;
            break;
        case PEL_IMG_JPEG: ;
            jpeg_easy_jpeg_t* jpeg = malloc(sizeof(jpeg_easy_jpeg_t));
            if (_jpeg_easy_read(fn, jpeg)) {
                handle->_err = PEL_ERR_JPEG_EASY;
                return -1;
            }

            *img = jpeg_to_pel(*jpeg);
            img->image_structure = jpeg;

            handle->_width = jpeg->width;
            handle->_width = jpeg->height;
            break;
        default:
            handle->_err = PEL_ERR_FORMAT;
            return -1;
    }

    return 0;
}

int _image_create_empty(char* filename, pel_image_type image_type, int width, int height)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    switch (image_type) {
        case PEL_IMG_PNG: ;
            if (_png_easy_create_empty(filename, width, height)) {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }
            break;
        case PEL_IMG_JPEG: ;
            if (_jpeg_easy_create_empty(filename, width, height)) {
                handle->_err = PEL_ERR_JPEG_EASY;
                return -1;
            }
            break;
        default:
            handle->_err = PEL_ERR_FORMAT;
            return -1;
    }
    return 0;
}

_pel_image_draw_cb_t _cur_draw_cb;

/* This callback converts a png-easy draw callback into a pel draw callback by being used as a callback to _png_easy_draw */
void image_png_draw_cb(int x, int y, png_bytep px) { _cur_draw_cb(x, y, png_px_to_pel_color(px)); }

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

    if (rect_start._x > rect_end._x) {
        int tmp = rect_start._x;
        rect_start._x = rect_end._x;
        rect_end._x = tmp;
    }
    
    if (rect_start._y > rect_end._y) {
        int tmp = rect_start._y;
        rect_start._y = rect_end._y;
        rect_end._y = tmp;
    }

    _cur_draw_cb = draw_cb;

    switch (handle->_image_in_type) {
        case PEL_IMG_PNG: ;
            if (_png_easy_draw(png_from_handle(handle, true), image_png_draw_cb, rect_start, rect_end)) {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }
            break;
        case PEL_IMG_JPEG: ;
            if (_jpeg_easy_draw(jpeg_from_handle(handle, true), image_png_draw_cb, rect_start, rect_end)) {
                handle->_err = PEL_ERR_JPEG_EASY;
                return -1;
            }
            break;
        default:
            handle->_err = PEL_ERR_FORMAT;
            return -1;
            break;
    }

    return 0;
}

/* Convert current pixels into pixels of each image type and save to handle. If in true, will load from image in to image out */
int load_pixels()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    for (int j = 0; j < handle->_height; j++) {
        for (int i = 0; i < handle->_width; i++) {
            pel_color_t color = handle->pixels[j][i];
            switch(handle->_image_out_type) {
                case PEL_IMG_PNG:
                    png_easy_png_t png = png_from_handle(handle, false);
                    png_bytep png_px = _png_easy_px(png, i, j);
                    _png_px_set(png_px, color);
                    break;
                case PEL_IMG_JPEG:
                    jpeg_easy_jpeg_t jpeg = jpeg_from_handle(handle, false);
                    uint8_t* jpeg_px = _jpeg_easy_px(jpeg, i, j);
                    _jpeg_px_set(jpeg_px, color);
                    break;
                default:
                    handle->_err = PEL_ERR_FORMAT;
                    return -1;
            }
        }
    }

    return 0;
}

int _image_write()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    if (load_pixels()) {
        handle->_err = PEL_ERR_UNKOWN;
        return -1;
    }

    switch (handle->_image_out_type) {
        case PEL_IMG_PNG: ;
            if (_png_easy_write(handle->_fn_out, png_from_handle(handle, false))) {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }
            break;
        case PEL_IMG_JPEG: ;
            if (_jpeg_easy_write(handle->_fn_out, jpeg_from_handle(handle, false))) {
                handle->_err = PEL_ERR_JPEG_EASY;
                return -1;
            }
            break;
        default:
            handle->_err = PEL_ERR_FORMAT;
            return -1;
    }

    return 0;
}
