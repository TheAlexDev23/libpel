#include <stdlib.h>
#include <stdio.h>

#include <pel.h>

int main()
{
    system("mkdir triangles");
    if (pel_init_create("triangles/triangle1.png", PEL_IMG_SOURCE_PNG, 100, 100))
    {
        fprintf(stderr, "Error durign pel init %s\n", pel_strerrno());
    }

    pel_cord_t p1 = PEL_CORD(0, 20), p2 = PEL_CORD(-20, 15), p3 = PEL_CORD(20, -15);

    if (pel_draw_triangle(PEL_COLOR_BLACK, p1, p2, p3))
    {
        fprintf(stderr, "Error during draw triangle %s\n", pel_strerrno());
    }

    pel_save();

    if (pel_init_create("triangles/triangle2.png", PEL_IMG_SOURCE_PNG, 100, 100))
    {
        fprintf(stderr, "Error durign pel init %s\n", pel_strerrno());
    }

    if (pel_draw_triangle_full(PEL_COLOR_BLACK, p1, p2, p3))
    {
        fprintf(stderr, "Error during draw triangle full %s\n", pel_strerrno());
    }

    pel_save();
}