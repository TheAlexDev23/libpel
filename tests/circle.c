#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int err()
{
    fprintf(stderr, "ERROR: %s\n", pel_strerrno());
    return -1;
}

int main()
{
    system("mkdir circles");

    if (pel_set_src_dest("circles/circle.png", "circles/circle.png"))
        return err();
    
    if (pel_init(100, 100))
        return err();

    if (pel_draw_circle(PEL_COLOR_WHITE, PEL_CORD(10, 10), 15))
        return err();

    if (pel_save())
        return err();

    if (pel_set_src_dest("circles/circle-full.png", "circles/circle-full.png"))
        return err();

    if (pel_init(100, 100))
        return err();

    if (pel_draw_circle_full(PEL_COLOR_WHITE, PEL_CORD(-10, 10), 15))
        return err();

    if (pel_save())
        return err();

    return 0;
}