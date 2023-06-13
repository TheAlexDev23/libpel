#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int main()
{
    system("mkdir fonts");
    int err = 0;
    if (pel_init("fonts/text1.png", "fonts/text1.png", 500, 300)) {
        fprintf(stderr, "Error during init: %s\n", pel_strerrno());
        return -1;
    }

    if (pel_draw_textbox(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 400, 30, PEL_TEXT_ALIGN_END, "End", PEL_CORD(0, 0)) || 
        pel_draw_textbox(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 250, 30, PEL_TEXT_ALIGN_CENTER, "Center", PEL_CORD(0, 0)) || 
        pel_draw_textbox(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 400, 30, PEL_TEXT_ALIGN_START, "Start", PEL_CORD(0, 0)))
    {
        fprintf(stderr, "Error during write: %s\n", pel_strerrno());
        return -1;
    }

    if (pel_save()) {
        fprintf(stderr, "Error during save %s", pel_strerrno());
        return -1;
    }

    return 0;
}