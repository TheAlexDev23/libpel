#include <stdlib.h>
#include <string.h>

#include "helper/ft-easy.h"
#include "helper/png-easy.h"

#include "pel.h"
#include "state.h"
#include "pixel.h"
#include "image.h"

int pel_draw_textbox(pel_color_t brush_color, pel_font_t font, int box_width, int line_offset, pel_text_align_t align, char* text, pel_cord_t cords)
{
    CHECK

    pel_handle_t* handle = _pel_get_cur_handle();

    if (_ft_easy_init()) return -1;

    get_xy_rel_img_center(cords._x, cords._y, handle);

    // All bitmaps
    pel_bitmap_t bms[strlen(text)];

    int* row_width = calloc(1, sizeof(int));

    // Iterator for iteration in row_width_diff. Reused throughout this function.
    int row_it = 0;

    int box_height = 0;
    // The height of the tallest letter in the current row
    int row_max_height = 0;

    cords._x -= box_width / 2;
    int offx = cords._x;

    for (int n = 0; n < strlen(text); n++)
    {
        pel_bitmap_t bm;
        if (_ft_easy_get_bm(font, text[n], &bm))
        {
            free(row_width);
            _ft_easy_exit();
            return -1;
        }

        bms[n] = bm;

        if (bm.height > row_max_height)
        {
            row_max_height = bm.height;
        }

        // Here we will have usually went to the next line
        if (row_width[row_it] + bm.width + offx >= box_width)
        {
            row_it++;
            row_width = realloc(row_width, sizeof(int) * (row_it + 1));
            row_width[row_it] = 0;
            box_height += line_offset;
            row_max_height = 0;
        }

        row_width[row_it] += bm.width;
    }

    cords._y -= box_height / 2;
    int offy = cords._y;

    // Element n indicates box_width - row_width of row n
    int row_width_diff[row_it + 1];

    for (int i = 0; i <= row_it; i++)
    {
        row_width_diff[i] = box_width - row_width[i];
    }

    row_it = 0;

    for (int n = 0; n < strlen(text); n++)
    {
        pel_bitmap_t bm = bms[n];

        /* Go to next line if next character wont fit */
        if (offx + bm.width >= box_width)
        {
            offx = cords._x;
            offy += line_offset;
            row_it++;
        }

        // Additional offset for text aligning
        int startx = 0;

        if (align == PEL_TEXT_ALIGN_END)
        {
            startx = row_width_diff[row_it];
        }
        else if (align == PEL_TEXT_ALIGN_CENTER)
        {
            startx = row_width_diff[row_it] / 2;
        }

        for (int j = 0; j < bm.height; j++)
        {
            for (int i = 0; i < bm.width; i++)
            {
                /* 
                * Accessing bottom row to draw from bottom.  
                * Like this, they all grown upwards from the same row 
                */
                brush_color.a = bm.bm[(bm.height - 1 - j) * bm.width + i].a;

                int x = offx + i + startx;
                int y = offy - j;

                _px_set(x, y, brush_color);
            }
        }

        offx += bm.width;
    }

    free(row_width);
    _ft_easy_exit();

    return 0; 
}