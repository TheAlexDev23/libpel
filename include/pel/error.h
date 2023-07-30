#ifndef _PEL_ERROR_H_INCLUDED
#define _PEL_ERROR_H_INCLUDED

typedef int PEL_ERROR;

typedef enum _pel_error_codes {
    PEL_ERR_UNKOWN = -1,
    PEL_SUCCESS = 0,
    PEL_ERR_HANDLE_NULL,
    PEL_ERR_SRC_DST_NOT_SPECIFIED,
    PEL_ERR_FORMAT,
    PEL_ERR_NO_FILE,
    PEL_ERR_DIMENSIONS,
    /* libpng related errors */
    PEL_ERR_PNG_EASY,
    /* libturbopng related errors*/
    PEL_ERR_JPEG_EASY,
    /* libwebp related errors */
    PEL_ERR_WEBP_EASY,
    /* FreeType related errors */
    PEL_ERR_FT_EASY,
    PEL_ERR_FT_FONT_NOT_FOUND,
    PEL_ERR_FT_LOAD_GLYPH,
    PEL_ERR_FT_RENDER_GLYPH,
} pel_error_codes_t;

/* Returns a pointer to a string with a description of the current error */
char* pel_strerrno();
/* Returns a pointer to a string with a description of the error */
char* pel_strerr(PEL_ERROR err);
/* Returns the current error number */
PEL_ERROR pel_errno();

#endif