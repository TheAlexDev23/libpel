#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir circles");
    if (pel_init("circles/circle.png", 100, 100))
        return -1;

    if (pel_draw_circle(PEL_COLOR_BLACK, PEL_CORD(0, 0), 15))
        return -1;

    if (pel_init("circles/circle-full.png", 100, 100))
        return -1;

    if (pel_draw_circle_full(PEL_COLOR_BLACK, PEL_CORD(0, 0), 15))
        return -1;

    return 0;
}