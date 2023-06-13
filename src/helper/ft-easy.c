#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <fontconfig/fontconfig.h>

#include "ft-easy.h"
#include "handle.h"
#include "state.h"
#include "pel.h"

FT_Library ft_lib;

int _ft_easy_init()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) goto Err;

    if (FT_Init_FreeType(&ft_lib)) goto Err;

    return 0;

    Err:
    if (handle != NULL) handle->_err = PEL_ERR_FT_EASY;
    
    return -1;
}

int _ft_easy_exit()
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    if (ft_lib == NULL || FT_Done_FreeType(ft_lib)) {
        handle->_err = PEL_ERR_FT_EASY;
        return -1;
    }

    return 0;
}

/* Convert FT bitmap to PEL bitmap */
void ft_bm_to_pel_bm(FT_Bitmap ft_bm, pel_bitmap_t* pel_bm)
{
    int height = ft_bm.rows, width = ft_bm.width;
    pel_bm->height = height;
    pel_bm->width = width;

    pel_bm->bm = (pel_color_t*)calloc(height * width, sizeof(pel_color_t));

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            uint8_t px = ft_bm.buffer[j * ft_bm.pitch + i];
            pel_bm->bm[j * width + i] = (pel_color_t){.r = 0, .g = 0, .b = 0, .a = px};
        }
    }
}

int get_font_base(char* filename, int size, pel_bitmap_t* bitmap, char character)
{
    FT_Face face;

    // At this point it should be already checked for null before
    pel_handle_t* handle = _pel_get_cur_handle();

    int err = 0;

    err = FT_New_Face(ft_lib,
                     filename,
                     0,
                     &face);

    if (err) {
        handle->_err = PEL_ERR_FT_FONT_NOT_FOUND;
        return -1;
    }

    FT_UInt glyph_index;
    FT_GlyphSlot slot = face->glyph;

    FT_Set_Pixel_Sizes(face, size, size);

    /* retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index(face, character);

    /* load glyph image into the slot (erase previous one) */
    err = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (err) {
        FT_Done_Face(face);
        handle->_err = PEL_ERR_FT_LOAD_GLYPH;
        return -1;
    }

    /* convert to an anti-aliased bitmap */
    err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (err) {
        FT_Done_Face(face);
        handle->_err = PEL_ERR_FT_RENDER_GLYPH;
        return -1;
    }

    FT_Bitmap bm = face->glyph->bitmap;

    ft_bm_to_pel_bm(bm, bitmap);

    FT_Done_Face(face);

    return 0;
}

int _ft_easy_get_bm(pel_font_t font, char character, pel_bitmap_t* bitmap)
{
    CHECK

    pel_handle_t* handle = _pel_get_cur_handle();

    /* Finding font location in a platform independent way */
    FcBool success = FcInit();
    if (!success) {
        handle->_err = PEL_ERR_FT_EASY;
        return -1;
    }

    FcPattern* pattern = FcPatternCreate();
    FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)font.font_family);
    FcPatternAddString(pattern, FC_STYLE, (const FcChar8*)font.font_style);

    FcResult result;
    FcPattern* matchedPattern = FcFontMatch(NULL, pattern, &result);

    FcChar8* fontFile;
    if (FcPatternGetString(matchedPattern, FC_FILE, 0, &fontFile) == FcResultMatch) {
        return get_font_base((char*)fontFile, font.size, bitmap, character);
    }
    else {
        handle->_err = PEL_ERR_FT_FONT_NOT_FOUND;
        return -1;
    }

    return 0;
}

int _ft_easy_get_bm_loc(char* filename, int size, char character, pel_bitmap_t* bitmap)
{
    CHECK
    return get_font_base(filename, size, bitmap, character);
}