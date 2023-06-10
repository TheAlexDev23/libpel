#include <stdio.h>

#include "helper/ft-easy.h"

#include "pel.h"
#include "state.h"

int pel_write(pel_color_t brush_color, char* font_location ,char* text, pel_cord_t cords)
{
    CHECK

    pel_bitmap_t bm;
    if (_ft_easy_get_bm_loc(font_location, 'I', &bm)) return -1;

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