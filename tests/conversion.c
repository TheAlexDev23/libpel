#include <stdlib.h>
#include <stdio.h>

#include "pel.h"

int err()
{
    fprintf(stderr, "ERROR: %s\n", pel_strerrno());
    return -1;
}

int main()
{
    system("mkdir conversions");

    if (pel_set_src_dest("conversions/file1.png", "conversions/file1.jpeg"))
        return err();

    if (pel_init(100, 100)) return err();

    if (pel_save()) return err();

    if (pel_set_src_dest("conversions/file1.jpeg", "conversions/file2.webp"))
        return err();

    /* Take into account file.webp should be black not transparent because file1.jpeg is black */
    if (pel_init(0, 0)) 
        return err();

    if (pel_save())
        return err();
}