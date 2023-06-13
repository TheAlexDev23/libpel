#include <stdlib.h>
#include <stdio.h>

#include "pel.h"

int main()
{
    system("mkdir rectangles");

    if (pel_init("rectangles/rectangle.png", "rectangles/rectangle.png", 100, 100)) {
        fprintf(stderr, "Error during init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_rectangle(PEL_COLOR_WHITE, PEL_CORD(-20, 5), PEL_CORD(20, -5))) {
        fprintf(stderr, "Error during draw rectangle %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    if (pel_init("rectangles/rectangle-full.png", "rectangles/rectangle-full.png" , 100, 100)) {
        fprintf(stderr, "Error during init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_rectangle_full(PEL_COLOR_WHITE, PEL_CORD(-20, 5), PEL_CORD(20, -5))) {
        fprintf(stderr, "Error during draw rectangle %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    return 0;
}