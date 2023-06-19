#include <stdlib.h>
#include <stdio.h>

#include "pel.h"

int err()
{
    fprintf(stderr, "ERROR: %s\n", pel_strerrno());
    return -1;
}

int main()
{
    system("mkdir rectangles");

    if (pel_set_src_dest("rectangles/rectangle.png", "rectangles/rectangle.png"))
        return err();

    if (pel_init(100, 100)) return err();

    if (pel_draw_rectangle(PEL_COLOR_WHITE, PEL_CORD(-20, 5), PEL_CORD(20, -5)))
        return err();

    if (pel_save()) return err();

    if (pel_set_src_dest("rectangles/rectangle-full.png", "rectangles/rectangle-full.png"))
        return err();

    if (pel_init(100, 100)) return err();

    if (pel_draw_rectangle_full(PEL_COLOR_WHITE, PEL_CORD(-20, 5), PEL_CORD(20, -5)))
        return err();

    if (pel_save()) return err();

    return 0;
}