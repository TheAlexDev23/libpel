#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir fonts");
    int err = 0;
    err = pel_init("fonts/text1.png", 100, 100);
    if (err)
    {
        fprintf(stderr, "ERROR (1): %s\n", pel_strerrno());
        return -1;
    }

    err = pel_write(PEL_COLOR_BLACK, "I", PEL_CORD(0, 0));
    if (err)
    {
        fprintf(stderr, "ERROR (2): %s\n", pel_strerrno());
        return -1;
    }

    return 0;
}