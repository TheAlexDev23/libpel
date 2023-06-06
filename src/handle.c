#include <stdlib.h>

#include "handle.h"

pel_handle_t* handle;

pel_handle_t* _pel_get_cur_handle()
{
    return handle;
}

void _pel_set_cur_handle(pel_handle_t* new_handle)
{
    if (handle != NULL)
        _pel_free_cur_handle();
    
    handle = new_handle;
}

void _pel_free_cur_handle()
{
    if (handle == NULL) return;

    free(handle->_fn);
    free(handle);
    
    handle = NULL;
}