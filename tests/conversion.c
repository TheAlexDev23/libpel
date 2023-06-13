#include <stdlib.h>
#include <stdio.h>

#include "pel.h"

int main()
{
    system("mkdir conversions");

    if (pel_init("conversions/file1.png", "conversions/file2.jpeg", 100, 100)) {
        fprintf(stderr, "Error during init %s", pel_strerrno());
        return -1;
    }

    if (pel_save()) {
        fprintf(stderr, "Error during save %s", pel_strerrno());
        return -1;
    }
}