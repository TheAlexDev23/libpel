/***************************************************************************************
* This code has been modified from open source software:
*    Title: libpng_test.c
*    Author: niw
*    Availability: https://gist.github.com/niw/5963798
*
***************************************************************************************/

#include "png-easy.h"

int png_easy_read(char* filename, png_easy_png_t* png_easy)
{
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return -1;

    png_infop info = png_create_info_struct(png); 
    if (!info) return -1;

    if (setjmp(png_jmpbuf(png))) return -1;

    png_init_io(png, fp);

    png_read_info(png, info);

    png_easy->width = png_get_image_width(png, info);
    png_easy->height = png_get_image_height(png, info);
    png_easy->color_type = png_get_color_type(png, info);
    png_easy->bit_depth = png_get_bit_depth(png, info);

    if (png_easy->bit_depth == 16)
        png_set_strip_16(png);

    if (png_easy->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (png_easy->color_type == PNG_COLOR_TYPE_GRAY && png_easy->bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (png_easy->color_type == PNG_COLOR_TYPE_RGB ||
        png_easy->color_type == PNG_COLOR_TYPE_GRAY ||
        png_easy->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (png_easy->color_type == PNG_COLOR_TYPE_GRAY ||
        png_easy->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_easy->row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * png_easy->height);
    for (int y = 0; y < png_easy->height; y++)
    {
        png_easy->row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, png_easy->row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    return 0;
}