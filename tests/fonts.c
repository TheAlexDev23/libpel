#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir fonts");
    int err = 0;
    if (pel_init_create("fonts/text1.png", PEL_IMG_SOURCE_PNG, 500, 300))
    {
        fprintf(stderr, "Error during init: %s\n", pel_strerrno());
        return -1;
    }

    if (pel_write(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 300, 30, "Hello", PEL_CORD(500/2 - 50, 300 / 2)))
    {
        fprintf(stderr, "Error during write: %s\n", pel_strerrno());
        return -1;
    }

    if (pel_save())
    {
        fprintf(stderr, "Error during save %s", pel_strerrno());
        return -1;
    }

    return 0;
}