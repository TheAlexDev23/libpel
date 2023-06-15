#include <stdio.h>
#include <stdlib.h>

#include <pel.h>

int err()
{
    fprintf(stderr, "ERROR: %s\n", pel_strerrno());
    return -1;
}

int main()
{
    system("mkdir fonts");

    if (pel_set_src_dest("fonts/text1.png", "fonts/text1.png"))
        return err();

    if (pel_init(500, 300)) return err();

    if (pel_draw_textbox(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 400, 30, PEL_TEXT_ALIGN_END, "End", PEL_CORD(0, 0)) || 
        pel_draw_textbox(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 250, 30, PEL_TEXT_ALIGN_CENTER, "Center", PEL_CORD(0, 0)) || 
        pel_draw_textbox(PEL_COLOR_BLACK, PEL_FONT("OpenSans", "Italic", 50), 400, 30, PEL_TEXT_ALIGN_START, "Start", PEL_CORD(0, 0)))
    {
        return err();
    }

    if (pel_save()) return err();

    return 0;
}