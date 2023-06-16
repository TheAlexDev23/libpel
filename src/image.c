#include <stdlib.h>
#include <string.h>

#include "helper/png-easy.h"
#include "helper/jpeg-easy.h"
#include "helper/webp-easy.h"

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
    } else if (strcmp(extension, ".webp") == 0) {
        return PEL_IMG_WEBP;
    }

    return PEL_IMG_CORRUPT;
}

/* Returns png_easy_png_t referenced by the handle->_img.image_structure */
png_easy_png_t png_from_handle(pel_handle_t* handle, bool in) 
{ 
    if (in)
        return *((png_easy_png_t*)handle->_in_image_struct); 
    else 
        return *((png_easy_png_t*)handle->_out_image_struct); 
}

/* Returns jpeg_easy_jpeg_t referenced by the handle->_img.image_structure */
jpeg_easy_jpeg_t jpeg_from_handle(pel_handle_t* handle, bool in) 
{
    if (in)
        return *((jpeg_easy_jpeg_t*)handle->_in_image_struct);
    else
        return *((jpeg_easy_jpeg_t*)handle->_out_image_struct);
}

/* Returns webp_easy_jpeg_t referenced by the handle->_img.image_structure */
webp_easy_webp_t webp_from_handle(pel_handle_t* handle, bool in) 
{
    if (in)
        return *((webp_easy_webp_t*)handle->_in_image_struct);
    else
        return *((webp_easy_webp_t*)handle->_out_image_struct);
}

int write_png_pixels_to_handle(png_easy_png_t png)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

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

    return 0;
}

int write_jpeg_pixels_to_handle(jpeg_easy_jpeg_t jpeg)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

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

    return 0;
}

int write_webp_pixels_to_handle(webp_easy_webp_t webp)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    handle->pixels = malloc(webp.height * sizeof(pel_color_t*));
    for (int i = 0; i < webp.height; i++) {
        handle->pixels[i] = malloc(webp.width * sizeof(pel_color_t));
    }

    for (int j = 0; j < webp.height; j++) {
        for (int i = 0; i < webp.width; i++) {
            handle->pixels[j][i] = webp_px_to_pel_color(
                _webp_easy_px(webp, i, j));
        }
    }

    return 0;
}

int _image_dimensions(char* fn, int* width, int* height)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    int _width = 0;
    int _height = 0;

    switch(_image_type(fn))
    {
        case PEL_IMG_PNG: ;
            png_easy_png_t* png = malloc(sizeof(png_easy_png_t));
            if (_png_easy_read(fn, png)) {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }

            _width = png->width;
            _height = png->height;

            free(png);
            break;
        case PEL_IMG_JPEG: ;
            jpeg_easy_jpeg_t* jpeg = malloc(sizeof(jpeg_easy_jpeg_t));
            if (_jpeg_easy_read(fn, jpeg)) {
                handle->_err = PEL_ERR_JPEG_EASY;
                return -1;
            }

            _width = jpeg->width;
            _height = jpeg->height;

            free(jpeg);
            break;
        case PEL_IMG_WEBP: ;
            webp_easy_webp_t* webp = malloc(sizeof(webp_easy_webp_t));
            if (_webp_easy_read(fn, webp)) {
                handle->_err = PEL_ERR_WEBP_EASY;
                return -1;
            }

            _width = webp->width;
            _height = webp->height;

            free(webp);
            break;
    }

    if (width) memcpy(width, &_width, sizeof(int));
    if (height) memcpy(height, &_height, sizeof(int));
    
    return 0;
}

/* Specifying "if in file" is important because we will eventually read both in and out files */
int _image_read(bool in)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    char* fn = in ? handle->_fn_in : handle->_fn_out;

    void** img = in ? &handle->_in_image_struct : &handle->_out_image_struct;

    /*
    * Load image data for each type of image
    * Write pixels if in file
    * Set handle data
    */
    switch(in ? handle->_image_in_type : handle->_image_out_type) {
        case PEL_IMG_PNG: ;
            png_easy_png_t* png = malloc(sizeof(png_easy_png_t));
            if (_png_easy_read(fn, png)) {
                handle->_err = PEL_ERR_PNG_EASY;
                return -1;
            }

            if (in) write_png_pixels_to_handle(*png);

            *img = png;

            handle->_width = png->width;
            handle->_height = png->height;
            break;
        case PEL_IMG_JPEG: ;
            jpeg_easy_jpeg_t* jpeg = malloc(sizeof(jpeg_easy_jpeg_t));
            if (_jpeg_easy_read(fn, jpeg)) {
                handle->_err = PEL_ERR_JPEG_EASY;
                return -1;
            }

            if (in) write_jpeg_pixels_to_handle(*jpeg);

            *img = jpeg;

            handle->_width = jpeg->width;
            handle->_width = jpeg->height;
            break;
        case PEL_IMG_WEBP: ;
            webp_easy_webp_t* webp = malloc(sizeof(webp_easy_webp_t));
            if (_webp_easy_read(fn, webp)) {
                handle->_err = PEL_ERR_WEBP_EASY;
                return -1;
            }

            if (in) write_webp_pixels_to_handle(*webp);

            *img = webp;

            handle->_width = webp->width;
            handle->_height = webp->width;
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
        case PEL_IMG_WEBP: ;
            if (_webp_easy_create_empty(filename, width, height)) {
                handle->_err = PEL_ERR_WEBP_EASY;
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

void image_jpeg_draw_cb(int x, int y, uint8_t* px) { _cur_draw_cb(x, y, jpeg_px_to_pel_color(px)); }

void image_webp_draw_cb(int x, int y, uint8_t* px) { _cur_draw_cb(x, y, webp_px_to_pel_color(px)); }

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
            if (_jpeg_easy_draw(jpeg_from_handle(handle, true), image_jpeg_draw_cb, rect_start, rect_end)) {
                handle->_err = PEL_ERR_JPEG_EASY;
                return -1;
            }
            break;
        case PEL_IMG_WEBP: ;
            if (_webp_easy_draw(webp_from_handle(handle, true), image_webp_draw_cb, rect_start, rect_end)) {
                handle->_err = PEL_ERR_WEBP_EASY;
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
                case PEL_IMG_PNG: ;
                    png_easy_png_t png = png_from_handle(handle, false);
                    png_bytep png_px = _png_easy_px(png, i, j);
                    _png_px_set(png_px, color);
                    break;
                case PEL_IMG_JPEG: ;
                    jpeg_easy_jpeg_t jpeg = jpeg_from_handle(handle, false);
                    uint8_t* jpeg_px = _jpeg_easy_px(jpeg, i, j);
                    _jpeg_px_set(jpeg_px, color);
                    break;
                case PEL_IMG_WEBP: ;
                    webp_easy_webp_t webp = webp_from_handle(handle, false);
                    uint8_t* webp_px = _webp_easy_px(webp, i, j);
                    _webp_px_set(webp_px, color);
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

    png_easy_png_t png_easy = png_from_handle(handle, false);

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
        case PEL_IMG_WEBP: ;
            if (_webp_easy_write(handle->_fn_out, webp_from_handle(handle, false))) {
                handle->_err = PEL_ERR_WEBP_EASY;
                return -1;
            }
            break;
        default:
            handle->_err = PEL_ERR_FORMAT;
            return -1;
    }

    return 0;
}
