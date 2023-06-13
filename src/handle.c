#include <stdlib.h>

#include "handle.h"

pel_handle_t* handle;

pel_handle_t* _pel_get_cur_handle()
{
    return handle;
}

void _pel_set_cur_handle(pel_handle_t* new_handle)
{
    if (handle != NULL) _pel_free_cur_handle();
    
    handle = new_handle;
}

void _pel_free_cur_handle()
{
    if (handle == NULL) return;

    free(handle->_fn_in);
    free(handle->_fn_out);

    if (handle->pixels == NULL || handle->_in_img.image_structure == NULL) goto skip_free;

    for (int i = 0; i < handle->_height; i++) {
        free(handle->pixels[i]);
    }

    free(handle->pixels);
    free(handle->_in_img.image_structure);

    skip_free:
    free(handle);
    
    handle = NULL;
}