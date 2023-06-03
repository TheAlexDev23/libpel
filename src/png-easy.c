/***************************************************************************************
* Parts of this code have been modified from open source software. Said software:
*    Title: libpng_test.c
*    Author: niw
*    Availability: https://gist.github.com/niw/5963798
*
***************************************************************************************/

#include <stdlib.h>
#include <string.h>

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

int png_easy_create_empty(char* filename, int width, int height)
{
    FILE *fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    fp = fopen(filename, "wb");
    if (!fp) return -1;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return -1;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return -1;
    }

    // Set the output file
    png_init_io(png_ptr, fp);

    // Set the image attributes
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        8, // Bit depth per channel (8-bit in this case)
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    // Write the PNG header
    png_write_info(png_ptr, info_ptr);

    // Allocate memory for the row data
    row = (png_bytep)malloc(4 * width * sizeof(png_byte));

    // Write empty rows (optional)
    memset(row, 0, 4 * width * sizeof(png_byte));
    for (int i = 0; i < height; i++)
    {
        png_write_row(png_ptr, row);
    }

    // End writing
    png_write_end(png_ptr, NULL);

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    free(row);

    return 0;
}

int png_easy_write(char* filename, png_easy_png_t png_easy)
{
    int y;

    FILE *fp = fopen(filename, "wb");
    if (!fp) return -1;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return -1;

    png_infop info = png_create_info_struct(png);
    if (!info)
        return -1;

    if (setjmp(png_jmpbuf(png))) return -1;

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png,
        info,
        png_easy.width, png_easy.height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    if (!png_easy.row_pointers) return -1;

    png_write_image(png, png_easy.row_pointers);
    png_write_end(png, NULL);

    for (int y = 0; y < png_easy.height; y++)
    {
        free(png_easy.row_pointers[y]);
    }
    free(png_easy.row_pointers);

    fclose(fp);

    png_destroy_write_struct(&png, &info);

    return 0;
}

int png_easy_draw(png_easy_png_t png, void (*draw_cb)(int x, int y, png_bytep px))
{
    for (int y = 0; y < png.height; y++)
    {
        png_bytep row = png.row_pointers[y];
        for (int x = 0; x < png.width; x++)
        {
            png_bytep px = &(row[x * 4]);
            draw_cb(x, y, px);
        }
    }

    return 0;
}