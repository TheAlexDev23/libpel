#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ft-easy.h"
#include "handle.h"
#include "state.h"
#include "pel.h"

int get_font_base(char* filename, pel_bitmap_t* bitmap, char character)
{
    FT_Library library;
    FT_Face face;

    int err = 0;
    err = FT_Init_FreeType( &library );
    if (err) return -1;

    err = FT_New_Face(library,
                     filename,
                     0,
                     &face);

    if (err)
    {
        _pel_get_cur_handle()->_err = PEL_ERR_FT_FONT_NOT_FOUND;
        return -1;
    }

    FT_UInt glyph_index;
    FT_GlyphSlot slot = face->glyph;

    /* retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index(face, character);

    /* load glyph image into the slot (erase previous one) */
    err = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (err)
    {
        _pel_get_cur_handle()->_err = PEL_ERR_FT_LOAD_GLYPH;
        return -1;
    }

    /* convert to an anti-aliased bitmap */
    err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (err)
    {
        _pel_get_cur_handle()->_err = PEL_ERR_FT_RENDER_GLYPH;
        return -1;
    }

    FT_Bitmap bm = face->glyph->bitmap;

    /* Convert FT bitmap to PEL bitmap */
    int height = bm.rows, width = bm.width;
    bitmap->height = height;
    bitmap->width = width;

    bitmap->bm = (pel_color_t*)calloc(height * width, sizeof(pel_px_t));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8_t px = bm.buffer[j * bm.pitch + i];
            bitmap->bm[j * width + i] = (pel_color_t){.r = 0, .g = 0, .b = 0, .a = px};
        }
    }

    return 0;
}

#define POSSIBLE_LOCATIONS_AMOUNT 6
int _tt_easy_get_bm(char* fontname, char character, pel_bitmap_t* bitmap)
{
    CHECK
    char* possible_locations[POSSIBLE_LOCATIONS_AMOUNT] = {
        "~/.fonts",
        "/usr/share/fonts/TTF/",
        "/usr/share/fonts/",
        "/usr/share/fonts/freetype/",
        "/usr/X11R6/lib/X11/fonts/ttfonts/", // RHL
        "/usr/X11R6/lib/X11/fonts/" //RHL
    };

    pel_handle_t* handle = _pel_get_cur_handle();

    for (int i = 0; i < POSSIBLE_LOCATIONS_AMOUNT; i++)
    {
        char* loc = possible_locations[i];
        char filename[strlen(loc) + strlen(fontname)];
        sprintf(filename, "%s%s", loc, fontname);
        int err = get_font_base(filename, bitmap, character);

        if (err)
        {
            if (handle->_err == PEL_ERR_FT_FONT_NOT_FOUND) continue;
            return -1;
        }

        return 0;
    }

    return -1;
}

int _tt_easy_get_bm_loc(char* filename, char character, pel_bitmap_t* bitmap)
{
    CHECK
    return get_font_base(filename, bitmap, character);
}