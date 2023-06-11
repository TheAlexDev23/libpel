#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <png.h>

#include "pel.h"
#include "helper/png-easy.h"
#include "handle.h"
#include "image.h"

extern int pel_init_create(char* filename, pel_image_source_type_t image_type, int height, int width)
{
    pel_handle_t* handle = calloc(1, sizeof(pel_handle_t));
    if (handle == NULL) return -1;
    _pel_set_cur_handle(handle);

    handle->_err = PEL_SUCCESS;

    handle->_image_source_type = image_type;

    handle->_fn = malloc(strlen(filename) + 1);
    strcpy(handle->_fn, filename),

    handle->_centerX = width / 2;
    handle->_centerY = height / 2;

    handle->_width = width;
    handle->_height = height;

    if (_image_create_empty(filename, image_type, height, width))
        return -1;

    pel_init(filename, image_type);

    return 0;
}

extern int pel_init(char* filename, pel_image_source_type_t image_type)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    /* This function can be called when handle is already initialized */
    if (handle == NULL)
    {
        handle = calloc(1, sizeof(pel_handle_t));
        if (handle == NULL) return -1;
        _pel_set_cur_handle(handle);
    }

    handle->_err = PEL_SUCCESS;

    handle->_image_source_type = image_type;

    handle->_fn = malloc(strlen(filename) + 1);
    strcpy(handle->_fn, filename);

    return _image_read();
}

int pel_save()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL ||
        _image_write())
    {
        return -1;
    }

    _pel_free_cur_handle();

    return 0;
}