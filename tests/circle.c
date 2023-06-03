#include <stdio.h>

#include <pel.h>

int main()
{
    if (pel_init("file.png", 100, 100))
    {
        printf("Error during init\n");
        return -1;
    }

    if (pel_draw_circle("file.png", PEL_COLOR_BLUE, 10, 6, 15))
    {
        printf("Error during draw circle\n");
        return -1;
    }

    return 0;
}