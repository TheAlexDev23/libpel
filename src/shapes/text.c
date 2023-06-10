#include <stdio.h>

#include "helper/ft-easy.h"

#include "pel.h"
#include "state.h"

int pel_write(pel_color_t brush_color, char* text, pel_cord_t cords)
{
    CHECK

    pel_bitmap_t bm;
    if (_tt_easy_get_bm("OpenSans-Regular.ttf", 'I', &bm))
    {
        _pel_get_cur_handle()->_err = PEL_ERR_FT_EASY;
        return -1;
    }

    for (int j = 0; j < bm.height; j++)
    {
        for (int i = 0; i < bm.width; i++)
        {
            printf("%i ", bm.bm[j * bm.width + i].a);
        }
        printf("\n");
    }

    return 0;
}