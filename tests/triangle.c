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
    system("mkdir triangles");

    if (pel_set_src_dest("triangles/triangle.png", "triangles/triangle.png"))
        return err();

    if (pel_init(100, 100)) return err();

    pel_cord_t p1 = PEL_CORD(0, 20), p2 = PEL_CORD(-20, 15), p3 = PEL_CORD(20, -15);

    if (pel_draw_triangle(PEL_COLOR_BLACK, p1, p2, p3))
        return err();

    if (pel_save()) return err();

    if (pel_set_src_dest("triangles/triangle-full.png", "triangles/triangle-full.png"))
        return err();

    if (pel_init(100, 100))
        return err();

    if (pel_draw_triangle_full(PEL_COLOR_BLACK, p1, p2, p3))
        return err();

    if (pel_save()) return err();

    return 0;
}