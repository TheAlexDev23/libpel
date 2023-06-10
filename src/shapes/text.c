#include <stdio.h>
#include <string.h>

#include "helper/ft-easy.h"
#include "helper/png-easy.h"

#include "pel.h"
#include "state.h"
#include "pixel.h"

int pel_write(pel_color_t brush_color, pel_font_t font, char* text, pel_cord_t cords)
{
    CHECK

    pel_handle_t* handle = _pel_get_cur_handle();

    png_easy_png_t png;
    if (_png_easy_read(handle->_fn, &png))
    {
        handle->_err = PEL_ERR_PNG_EASY;
        return -1;
    }

    int offy = cords._y, offx = cords._x;

    for (int n = 0; n < strlen(text); n++)
    {
        pel_bitmap_t bm;
        if (_ft_easy_get_bm(font, text[n], &bm)) return -1;

        /* Drawing rom reverse allows to allign characters fonts to the bottom rather than to the top */
        for (int j = 0; j < bm.height; j++)
        {
            for (int i = 0; i < bm.width; i++)
            {
                brush_color.a = bm.bm[(bm.height - 1 - j) * bm.width + i].a;

                int x = offx + i;
                int y = offy - j;

                _px_set(_png_easy_px(png, x, y), brush_color);
            }
        }

        offx += bm.width;
    }

    if (_png_easy_write(handle->_fn, png))
    {
        handle->_err = PEL_ERR_PNG_EASY;
        return -1;
    }

    return 0;
}