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
#include "image.h"
#include "handle.h"
#include "pixel.h"

int png_bit_depth = 8;
int png_color_type = PNG_COLOR_TYPE_RGBA;

/* Returns 0 if color_type is not supported otherwise returns libpng format */
int supported_format(int color_type)
{
    switch (color_type)
    {
        case PEL_PNG_FORMAT_RGBA:
            return PNG_COLOR_TYPE_RGBA;
        case PEL_PNG_FORMAT_RGB:
            return PNG_COLOR_TYPE_RGB;
        default:
            return 0;
    }
}

int pel_conf_png(int bit_depth, int color_type)
{
    /* We can only configure before calling pel_init and/or after pel_save */
    if (_pel_get_cur_handle() != NULL) return -1;

    int format;
    if ((format = supported_format(color_type)) == 0) return -1;

    png_bit_depth = bit_depth;
    png_color_type = format;
    return 0;
}

int _png_easy_read(char* filename, png_easy_png_t* png)
{
    FILE *fp = fopen(filename, "rb");

    png_structp png_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_struct) return -1;

    png_infop info = png_create_info_struct(png_struct); 
    if (!info) return -1;

    if (setjmp(png_jmpbuf(png_struct))) return -1;

    png_init_io(png_struct, fp);

    png_read_info(png_struct, info);

    png->width = png_get_image_width(png_struct, info);
    png->height = png_get_image_height(png_struct, info);
    png->color_type = png_get_color_type(png_struct, info);
    png->bit_depth = png_get_bit_depth(png_struct, info);

    if (png->bit_depth == 16)
        png_set_strip_16(png_struct);

    if (png->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_struct);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (png->color_type == PNG_COLOR_TYPE_GRAY && png->bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_struct);

    if (png_get_valid(png_struct, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_struct);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (png->color_type == PNG_COLOR_TYPE_RGB ||
        png->color_type == PNG_COLOR_TYPE_GRAY ||
        png->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png_struct, 0xFF, PNG_FILLER_AFTER);

    if (png->color_type == PNG_COLOR_TYPE_GRAY ||
        png->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_struct);

    png_read_update_info(png_struct, info);

    png->row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * png->height);
    for (int y = 0; y < png->height; y++)
    {
        png->row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_struct, info));
    }

    png_read_image(png_struct, png->row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png_struct, &info, NULL);

    return 0;
}

int _png_easy_create_empty(char* filename, int width, int height)
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
    if (!info_ptr) {
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
        png_bit_depth,
        png_color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    // Write the PNG header
    png_write_info(png_ptr, info_ptr);

    // Allocate memory for the row data
    row = (png_bytep)malloc(4 * width * sizeof(png_byte));

    // Write empty rows
    memset(row, 0, 4 * width * sizeof(png_byte));
    for (int i = 0; i < height; i++) {
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

int _png_easy_write(char* filename, png_easy_png_t png)
{
    int y;

    FILE *fp = fopen(filename, "wb");
    if (!fp) return -1;

    png_structp png_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_struct) return -1;

    png_infop info = png_create_info_struct(png_struct);
    if (!info) return -1;

    if (setjmp(png_jmpbuf(png_struct))) return -1;

    png_init_io(png_struct, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png_struct,
        info,
        png.width, png.height,
        png.bit_depth,
        png.color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_struct, info);

    if (png.color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_struct, 0, PNG_FILLER_AFTER);

    if (!png.row_pointers) return -1;

    png_write_image(png_struct, png.row_pointers);
    png_write_end(png_struct, NULL);

    for (int y = 0; y < png.height; y++)
    {
        free(png.row_pointers[y]);
    }
    free(png.row_pointers);

    fclose(fp);

    png_destroy_write_struct(&png_struct, &info);

    return 0;
}

int _png_easy_draw(png_easy_png_t png, _png_easy_draw_cb draw_cb, pel_cord_t rect_start, pel_cord_t rect_end)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    for (int y = rect_start._y; y < rect_end._y; y++) {
        for (int x = rect_start._x; x < rect_end._x; x++) {
            int rx = x, ry = y;
            get_xy_rel_img_center(rx, ry, handle);

            png_bytep px = _png_easy_px(png, rx, ry);
            if (px != NULL) draw_cb(rx, ry, px);
        }
    }

    return 0;
}

png_bytep _png_easy_px(png_easy_png_t png, int x, int y)
{
    if (y >= png.height || x >= png.width) return NULL;

    png_bytep row = png.row_pointers[y];
    return &(row[x * 4]);
}

void _png_px_set_def_color(png_bytep px)
{
    if (px == NULL) return;

    pel_color_t cur_color = _get_color();

    px[0] = cur_color.r;
    px[1] = cur_color.g;
    px[2] = cur_color.b;
    px[3] = cur_color.a;
}

void _png_px_set(png_bytep px, pel_color_t color)
{
    if (px == NULL) return;

    _set_color(color);
    _png_px_set_def_color(px);
}

/* Converts a png pixel into a pel color */
pel_color_t png_px_to_pel_color(png_bytep px)
{
    pel_color_t pel_color;
    if (px == NULL) return pel_color;

    pel_color.r = px[0];
    pel_color.g = px[1];
    pel_color.b = px[2];
    pel_color.a = px[3];
    
    return pel_color;
}
