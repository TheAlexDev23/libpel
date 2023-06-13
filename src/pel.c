#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <png.h>

#include "pel.h"
#include "helper/png-easy.h"
#include "handle.h"
#include "image.h"

int pel_init(char* fn_in, char* fn_out, int width, int height)
{
    pel_handle_t* handle = calloc(1, sizeof(pel_handle_t));
    if (handle == NULL) return -1;
    _pel_set_cur_handle(handle);

    handle->_err = PEL_SUCCESS;

    handle->_image_in_type = _image_type(fn_in);
    handle->_image_out_type = _image_type(fn_out);

    handle->_fn_in = malloc(strlen(fn_in) + 1);
    strcpy(handle->_fn_in, fn_in);
    handle->_fn_out = malloc(strlen(fn_out) + 1);
    strcpy(handle->_fn_out, fn_out);

    handle->_centerX = width / 2;
    handle->_centerY = height / 2;

    handle->_width = width;
    handle->_height = height;

    if (access(fn_in, F_OK)) {
        if (_image_create_empty(fn_in, handle->_image_in_type, width, height)) return -1;
    }

    if (access(fn_out, F_OK)) {
        if (_image_create_empty(fn_out, handle->_image_out_type, width, height)) return -1;
    }

    if (_image_read(true) || _image_read(false)) return -1;

    return 0;
}

int pel_save()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL || _image_write()) return -1;

    _pel_free_cur_handle();

    return 0;
}