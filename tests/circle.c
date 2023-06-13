#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir circles");
    if (pel_init("circles/circle.png", "circles/circle.png", 100, 100)) {
        fprintf(stderr, "ERROR: failed init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_circle(PEL_COLOR_WHITE, PEL_CORD(10, 10), 15)) {
        fprintf(stderr, "ERROR: failed draw circle %s\n", pel_strerrno());
        return -1;
    }

    if (pel_save()) {
        fprintf(stderr, "Error during save %s", pel_strerrno());
        return -1;
    }

    if (pel_init("circles/circle-full.png", "circles/circle-full.png", 100, 100)) {
        fprintf(stderr, "ERROR: failed init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_circle_full(PEL_COLOR_WHITE, PEL_CORD(10, -10), 15)) {
        fprintf(stderr, "ERROR: failed draw circle full %s\n", pel_strerrno());
        return -1;
    }

    if (pel_save()) {
        fprintf(stderr, "Error during save %s", pel_strerrno());
        return -1;
    }

    return 0;
}