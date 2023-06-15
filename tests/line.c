#include <stdlib.h>
#include <stdio.h>

#include <pel.h>

int err()
{
    fprintf(stderr, "ERROR: %s\n", pel_strerrno());
    return -1;
}

int main()
{
    system("mkdir lines");
    
    if (pel_set_src_dest("lines/line1.png", "lines/line1.png"))
        return err();

    if (pel_init(100, 100)) return err();

    if (pel_draw_line(PEL_COLOR_RED, PEL_CORD(-50, 50), PEL_CORD(50, -50)) ||
        pel_draw_line(PEL_COLOR_GREEN, PEL_CORD(-50, -50), PEL_CORD(50, 50)) ||
        pel_draw_line(PEL_COLOR_BLACK, PEL_CORD(0, 50), PEL_CORD(0, -50)) ||
        pel_draw_line(PEL_COLOR_WHITE, PEL_CORD(-50, 0), PEL_CORD(50, 0)))
    {
        return err();
    }

    if (pel_save()) return err();

    return 0;   
}