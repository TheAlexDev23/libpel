#include <stdlib.h>
#include <stdio.h>

#include <pel.h>

int main()
{
    system("mkdir lines");
    if (pel_init_create("lines/line1.png", PEL_IMG_SOURCE_PNG, 100, 100)) {
        fprintf(stderr, "Error during init %s", pel_strerrno());
        return -1;
    }

    if (pel_draw_line(PEL_COLOR_RED, PEL_CORD(-50, 50), PEL_CORD(50, -50)) ||
        pel_draw_line(PEL_COLOR_GREEN, PEL_CORD(-50, -50), PEL_CORD(50, 50)) ||
        pel_draw_line(PEL_COLOR_BLACK, PEL_CORD(0, 50), PEL_CORD(0, -50)) ||
        pel_draw_line(PEL_COLOR_WHITE, PEL_CORD(-50, 0), PEL_CORD(50, 0)))
    {
        fprintf(stderr, "Error during draw line %s", pel_strerrno());
        return -1;
    }

    if (pel_save()) {
        fprintf(stderr, "Error during save %s", pel_strerrno());
        return -1;
    }

    return 0;   
}