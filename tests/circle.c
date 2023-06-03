#include <stdio.h>

#include <pel.h>

int main()
{
    if (pel_init("file.png", 100, 100))
    {
        printf("Error during init\n");
        return -1;
    }

    if (pel_draw_circle("file.png", 0, 0, 23))
    {
        printf("Error during draw circle\n");
        return -1;
    }

    return 0;
}