#include <ft2build.h>
#include FT_FREETYPE_H

#include "tt-easy.h"

int get_font_base(char* filename, pel_bitmap_t* bitmap)
{
    FT_Library library;
    FT_Face face;

    int err = 0;
    err = FT_Init_FreeType( &library );
    if (err) return -1;

    err = FT_New_Face(library,
                     filename,
                     0,
                     &face );

    if (err) return -1;

    FT_UInt glyph_index;
    FT_GlyphSlot slot = face->glyph;

    /* retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index(face, text[n]);

    /* load glyph image into the slot (erase previous one) */
    err = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (err)
        continue; // ignore errors

    /* convert to an anti-aliased bitmap */
    err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL );
    if (err)
        continue;
        
    return 0;
}

int _tt_easy_get_bm(char* fontname, pel_bitmap_t* bitmap)
{

    return 0;
}

int _tt_easy_get_bm_loc(char* filename, pel_bitmap_t* bitmap)
{
    return 0;
}