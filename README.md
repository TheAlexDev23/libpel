# PEL: Photo Editing Library

C library that allows image manipulation and image editing. Originally written for a school project.

Current supported filetypes include:
- PNG

## Installation

`install.sh` adds the pkgconf file of libpel to the system, configures the project and builts it.
After building, copies header and binaries into /usr/include/pel and /usr/bin.
Requires to be run with root privilegies.

```bash
git clone https://github.com/TheAlexDev23/libpel
cd libpel
chmod +x ./install.sh
sudo ./install.sh
```

## Using library
If running the default install script, a pkgconf file should have been created. You can build your project and add this library by adding the output of `pkgconf pel --libs -cflags` to the compile options of your prject.

Example
```bash
cc mycode.c `pkgconf pel --libs -cflas` -o mybin
```

## Basic API overview
PEL has an internal handle referencing all the needed data about the current image. The user only needs to call the initialazation function.

```C
#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    // Silence output in case circles dir already exists
    system("mkdir circles > /dev/null");

    if (pel_init("circles/circle.png", PEL_IMG_SOURCE_PNG, 100, 100))
    {
        fprintf(stderr, "ERROR: failed init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_circle(PEL_COLOR_WHITE, PEL_CORD(10, 10), 15))
    {
        fprintf(stderr, "ERROR: failed draw circle %s\n", pel_strerrno());
        return -1;
    }

    pel_save(); // Necessary when finished editing or about to start editing another file. Not calling will result in loss of data.

    if (pel_init("circles/circle-full.png", PEL_IMG_SOURCE_PNG, 100, 100))
    {
        fprintf(stderr, "ERROR: failed init %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_circle_full(PEL_COLOR_WHITE, PEL_CORD(10, -10), 15))
    {
        fprintf(stderr, "ERROR: failed draw circle full %s\n", pel_strerrno());
        return -1;
    }

    pel_save();

    return 0;
}
```

The [tests folder](/tests/) has a series of tests used in development to test the functionality of the library. They are good examples of all the functionaity of libpel.

## Documentation
Comming soon. I'm currrently only focusing on just the development but I'll probably add API and internal (structure realated, useful for development) documentation after I finish the base functionality.

