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
    system("mkdir custom_formats");

    if (pel_conf_png(8, PEL_PNG_FORMAT_RGBA)) return err();

    if (pel_set_src_dest("custom_formats/rgba.png", "custom_formats/rgba.png"))
        return err();

    if (pel_init(100, 100)) return err();
    
    if (pel_save()) return err();

    if (pel_conf_png(8, PEL_PNG_FORMAT_RGB)) return err();

    if (pel_set_src_dest("custom_formats/rgb.png", "custom_formats/rgb.png"))
        return err();

    if (pel_init(100, 100)) return err();
    
    if (pel_save()) return err();

    if (pel_conf_jpeg(50)) return err();

    if (pel_set_src_dest("custom_formats/quality50.jpeg", "custom_formats/quality50.jpeg"))
        return err();

    if (pel_init(100, 100)) return err();
    
    if (pel_save()) return err();

    return 0;
}