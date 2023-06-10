#include <stddef.h>

#include "pel.h"

#include "handle.h"

char* pel_strerrno()
{
    return pel_strerr(pel_errno());
}

char* pel_strerr(PEL_ERROR err)
{
    switch(err)
    {
        case PEL_ERR_HANDLE_NULL:
            return "PEL internal handle is NULL. Make sure to initialize before use";
        case PEL_ERR_PNG_EASY:
            return "Error with internal PEL libpng wrapper";
        case PEL_ERR_TT_EASY:
            return "Error with internal PEL truetype wrapper";
        default:
            return "Unkown error";
    }
}

PEL_ERROR pel_errno()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL)
    {
        return PEL_ERR_HANDLE_NULL;
    }

    return handle->_err;
}