#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir circles");
    if (pel_init_create("circles/circle.png", PEL_IMG_SOURCE_PNG, 100, 100))
    {
        fprintf(stderr, "ERROR: failed init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_circle(PEL_COLOR_WHITE, PEL_CORD(10, 10), 15))
    {
        fprintf(stderr, "ERROR: failed draw circle %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    if (pel_init_create("circles/circle-full.png", PEL_IMG_SOURCE_PNG, 100, 100))
    {
        fprintf(stderr, "ERROR: failed init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_circle_full(PEL_COLOR_WHITE, PEL_CORD(10, -10), 15))
    {
        fprintf(stderr, "ERROR: failed draw circle full %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    return 0;
}