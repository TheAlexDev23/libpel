#include <stddef.h>

#include "pel.h"

#include "handle.h"

char* pel_strerrno() { return pel_strerr(pel_errno()); }

char* pel_strerr(PEL_ERROR err)
{
    switch(err) {
        case PEL_ERR_HANDLE_NULL:
            return "PEL internal handle is NULL. Make sure to initialize before use";
        case PEL_ERR_FORMAT:
            return "The provided format is not supported or recognized by PEL";
        case PEL_ERR_NO_FILE:
            return "A file needs to exist when it doesn't";
        case PEL_ERR_DIMENSIONS:
            return "Requested and file dimensions do not match";
        case PEL_ERR_PNG_EASY:
            return "Error with internal PEL libpng wrapper";
        case PEL_ERR_JPEG_EASY:
            return "Error with internal PEL libjpeg wrapper";
        case PEL_ERR_FT_EASY:
            return "Error with internal PEL freetype wrapper";
        case PEL_ERR_FT_FONT_NOT_FOUND:
            return "Internal PEL FreeType wrapper: font not found";
        case PEL_ERR_FT_LOAD_GLYPH:
            return "Internal PEL FreeType wrapper: could not load glyph";
        case PEL_ERR_FT_RENDER_GLYPH:
            return "Internal PEL FreeType wrapper: could not render glyph into pixel data";
        case PEL_SUCCESS:
            return "No errors";
        case PEL_ERR_UNKOWN:
            return "Specified unkown error";
        default:
            return "Unspecified unkown error";
    }
}

PEL_ERROR pel_errno()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return PEL_ERR_HANDLE_NULL;

    return handle->_err;
}