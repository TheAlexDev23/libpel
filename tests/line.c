#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir lines");
    if (pel_init("lines/line1.png", 100, 100))
        return -1;

    if (pel_draw_line(PEL_COLOR_BLACK, PEL_CORD(0, 0), PEL_CORD(15, 13)))
        return -1;

    return 0;   
}