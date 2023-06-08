#include <stdlib.h>
#include <stdio.h>

#include <pel.h>

int main()
{
    system("mkdir lines");
    if (pel_init("lines/line1.png", 100, 100))
        return -1;
    if (pel_draw_line(PEL_COLOR_WHITE, PEL_CORD(0, 0), PEL_CORD(20, 7)))
        return -1;

    return 0;   
}