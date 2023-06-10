#include <stdio.h>

#include "helper/ft-easy.h"

#include "pel.h"

int pel_write(pel_color_t brush_color, char* text, pel_cord_t cords)
{
    pel_bitmap_t bm;
    _tt_easy_get_bm("OpenSans-Regular.ttf", 'I', &bm);
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