#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pel.h>

int err()
{
    fprintf(stderr, "ERROR: %s\n", pel_strerrno());
    return -1;
}

int main()
{
    system("mkdir filters");
    /* Default image I use for filter testing. It is not tracked by git and on instances where it isn't installed an empty image is used */
    if (!access("linux.png", F_OK)) {
        if (pel_set_src_dest("linux.png", "filters/grayscale.png"))
            return err();

        if (pel_init(0, 0))
            return err();
    } else {
        if (pel_set_src_dest("filters/base.png", "filters/grayscale.png"))
            return err();
        
        if (pel_init(100, 100))
            return err();
    }
    
    if (pel_filter_graysacle())
        return err();

    if (pel_save())
        return err();

    return 0;
}