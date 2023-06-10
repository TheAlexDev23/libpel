#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <png.h>

#include "pel.h"
#include "helper/png-easy.h"
#include "handle.h"

pel_color_t color;
int pel_init(char* filename, int height, int width)
{
    if (access(filename, F_OK))
    {
        if (_png_easy_create_empty(filename, height, width))
            return -1;
    }

    pel_handle_t* handle = calloc(1, sizeof(pel_handle_t));
    handle->_err = PEL_SUCCESS;

    handle->_fn = malloc(strlen(filename) + 1);
    strcpy(handle->_fn, filename),

    handle->_centerX = width / 2;
    handle->_centerY = height / 2;

    handle->_width = width;
    handle->_height = height;

    _pel_set_cur_handle(handle);

    return 0;
}