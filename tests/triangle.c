#include <stdlib.h>
#include <stdio.h>

#include <pel.h>

int main()
{
    system("mkdir triangles");
    if (pel_init("triangles/triangle.png", "triangles/triangle.png", 100, 100)) {
        fprintf(stderr, "Error durign pel init %s\n", pel_strerrno());
        return -1;
    }

    pel_cord_t p1 = PEL_CORD(0, 20), p2 = PEL_CORD(-20, 15), p3 = PEL_CORD(20, -15);

    if (pel_draw_triangle(PEL_COLOR_BLACK, p1, p2, p3)) {
        fprintf(stderr, "Error during draw triangle %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    if (pel_init("triangles/triangle-full.png", "triangles/triangle-full.png", 100, 100)) {
        fprintf(stderr, "Error durign pel init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_triangle_full(PEL_COLOR_BLACK, p1, p2, p3)) {
        fprintf(stderr, "Error during draw triangle full %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    return 0;
}