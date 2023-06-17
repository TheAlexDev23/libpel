#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <png.h>

#include "pel.h"
#include "helper/png-easy.h"
#include "handle.h"
#include "image.h"

int pel_set_src_dest(char* fn_src, char* fn_dst)
{
    pel_handle_t* handle = calloc(1, sizeof(pel_handle_t));
    if (handle == NULL) return -1;
    _pel_set_cur_handle(handle);

    handle->_err = PEL_SUCCESS;

    handle->_image_in_type = _image_type(fn_src);
    handle->_image_out_type = _image_type(fn_dst);

    handle->_fn_in = malloc(strlen(fn_src) + 1);
    strcpy(handle->_fn_in, fn_src);
    handle->_fn_out = malloc(strlen(fn_dst) + 1);
    strcpy(handle->_fn_out, fn_dst);

    return 0;
}

/* Returns 1 if fn image doesn't have the specified height/width. -1 on error*/
int verify_image(char* fn, int height, int width) 
{
    int _width, _height;
    if (_image_dimensions(fn, &_width, &_height)) {
        return -1;
    }

    if (_width == width && _height == height) return 0;

    return 1;
}

int pel_init(int width, int height)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    int fn_in_exists = !access(handle->_fn_in, F_OK);
    int fn_out_exists = !access(handle->_fn_out, F_OK);

    handle->_err = PEL_SUCCESS;

    if (width == 0 && fn_in_exists) {
        if (_image_dimensions(handle->_fn_in, &width, NULL))
            return -1;
    } else if (!fn_in_exists && width == 0) {
        handle->_err = PEL_ERR_NO_FILE;
        return -1;
    }

    if (height == 0 && fn_in_exists) {
        if (_image_dimensions(handle->_fn_in, NULL, &height))
            return -1;
    } else if (!fn_in_exists && height == 0) {
        handle->_err = PEL_ERR_NO_FILE;
        return -1;
    }

    if (!fn_in_exists) {
        if (_image_create_empty(handle->_fn_in, handle->_image_in_type,
                                width, height)) {
            return -1;
        }
    }

    if (!fn_out_exists) {
        if (_image_create_empty(handle->_fn_out, handle->_image_out_type, 
                                width, height)) {
            return -1;
        }
    }

    int fn_in_dimensions = verify_image(handle->_fn_in, height, width);
    int fn_out_dimensions = verify_image(handle->_fn_out, height, width);

    if (fn_in_dimensions || fn_out_dimensions) {
        /* Error code will be specified in the handle if it was an internal error */
        if (fn_in_dimensions < 0 || fn_out_dimensions < 0)
            return -1;

        /* Otherwise it's just wrong dimensions*/
        handle->_err = PEL_ERR_DIMENSIONS;
        return -1;
    }

    handle->_width = width;
    handle->_height = height;

    handle->_centerX = width / 2;
    handle->_centerY = height / 2;

    if (_image_read(true) || _image_read(false))
        return -1;

    return 0;
}

int pel_save()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL || _image_write()) return -1;

    _pel_free_cur_handle();

    _pel_set_cur_handle(NULL);

    return 0;
}