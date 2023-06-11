#include <stdio.h>

#include <pel.h>

int main()
{
    // No init to check error handling
    // pel_init();

    // Checking if will return error
    if (!pel_draw_circle(PEL_COLOR_BLACK, PEL_CORD(0, 0), 5))
    {
        fprintf(stderr, "PEL returned success when shouldn't\n");
        return -1;
    }

    return 0;
}