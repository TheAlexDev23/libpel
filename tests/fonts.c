#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir fonts");
    int err = 0;
    err = pel_init_create("fonts/text1.png", PEL_IMG_SOURCE_PNG, 500, 300);
    if (err)
    {
        fprintf(stderr, "ERROR (1): %s\n", pel_strerrno());
        return -1;
    }

    err = pel_write(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 300, 30, "Hello", PEL_CORD(500/2 - 50, 300 / 2));

    if (err)
    {
        fprintf(stderr, "ERROR (2): %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    return 0;
}