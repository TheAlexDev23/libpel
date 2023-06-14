# PEL: Photo Editing Library

C library that allows image manipulation and image editing. Originally written for a school project.

Current supported filetypes include:
- PNG
- JPEG

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
If running the default install script, a pkgconf file should have been created. You can build your project and add this library by adding the output of `pkgconf pel --libs -cflags` to the compile options of your project.

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

The [tests folder](/tests/) has a series of tests used in development to test the functionality of the library. They are good examples of the functionaity and API of PEL.

## Documentation
Comming soon. I'm currrently only focusing on just the development but I'll probably add API and internal (structure relaated, useful for development) documentation after I finish the base functionality. But as a general overview, the API has these functions:

### Functions

**Init Exit**
- pel_init
- pel_save

**Shapes**

- pel_draw_circle
- pel_draw_circle_full
- pel_draw_rectangle
- pel_draw_rectangle_full
- pel_draw_line
- pel_draw_textbox
- pel_draw_triangle
- pel_draw_triangle_full

**Error handling**

Each PEL function returns -1 on error. Error codes and errors can be accessed with:
- pel_errno -> current error code
- pel_sterr -> description of error
- pel_strerrno -> description of current error code

**Structs, enums and macros**

- pel_color_t -> rgba color
- PEL_COLOR_(BLACK/WHITE/RED/GREEN/BLUE) -> macros facilitating the creatoin of pel_color_t
- pel_bitmap_t
- pel_cord_t -> 2D coordinate
- PEL_CORD -> macro facilitating the creation of pel_cord_t
- pel_font_t -> represenatation of a font
- PEL_FONT -> macro facilitating the creation of fonts
- pel_text_align_t -> enum representing alignment of text
