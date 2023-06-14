#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>

#include <jpeglib.h>

#include "pel.h"
#include "jpeg-easy.h"
#include "handle.h"
#include "pixel.h"

int _jpeg_easy_create_empty(char* filename, int width, int height)
{
    // TODO: Add custom compression options for _jpeg_easy_create_empty
    
    /* TurboJPEG doesn't allow writing headers, so we need to use libjpeg */
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, PEL_JPEG_EASY_QUALITY, TRUE);

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
        return -1;
    jpeg_stdio_dest(&cinfo, fp);

    jpeg_start_compress(&cinfo, true);

    // Write empty scanlines
    uint8_t* row_pointer[1];
    int row_size = width * cinfo.input_components;
    uint8_t dummy_scanline[row_size];
    /* Fill with 0 */
    memset(dummy_scanline, 0, row_size);

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = dummy_scanline;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(fp);

    return 0;
}

int _jpeg_easy_read(char *filename, jpeg_easy_jpeg_t *jpeg)
{
    struct jpeg_decompress_struct info;

    struct jpeg_error_mgr err;
    info.err = jpeg_std_error(&err);

    jpeg_create_decompress(&info);

    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) return -1;
    jpeg_stdio_src(&info, fp);

    jpeg_read_header(&info, true);
    jpeg_start_decompress(&info);

    int width = info.output_width;
    int height = info.output_height;
    int row_size = info.output_width * info.output_components;

    jpeg->width = width;
    jpeg->height = height;
    jpeg->row_size = row_size;

    /* Length of all pixel data in bytes */
    int image_bytes = row_size * height;

    jpeg->pixels = malloc(image_bytes);

    uint8_t* row_buffer[1];

    while (info.output_scanline < height) {
        row_buffer[0] = &(jpeg->pixels[row_size * info.output_scanline]);
        jpeg_read_scanlines(&info, row_buffer, 1);
    }

    jpeg_finish_decompress(&info);
    jpeg_destroy_decompress(&info);

    fclose(fp);

    return 0;
}

int _jpeg_easy_write(char *filename, jpeg_easy_jpeg_t jpeg)
{
    int quality = PEL_JPEG_EASY_QUALITY;
    int width = jpeg.width;
    int height = jpeg.height;
    int row_size = jpeg.row_size;

    struct jpeg_compress_struct cinfo;

    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
        return -1;
    jpeg_stdio_dest(&cinfo, fp);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, FALSE);

    uint8_t* row_pointer[1];

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &(jpeg.pixels[row_size * cinfo.next_scanline]);
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(fp);

    return 0;
}

int _jpeg_easy_draw(jpeg_easy_jpeg_t jpeg, _jpeg_easy_draw_cb draw_cb, pel_cord_t rect_start, pel_cord_t rect_end)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    for (int y = rect_start._y; y < rect_end._y; y++) {
        for (int x = rect_start._x; x < rect_end._x; x++) {
            int rx = x, ry = y;
            get_xy_rel_img_center(rx, ry, handle);

            uint8_t* px = _jpeg_easy_px(jpeg, rx, ry);
            if (px != NULL) draw_cb(rx, ry, px);
        }
    }

    return 0;
}

uint8_t* _jpeg_easy_px(jpeg_easy_jpeg_t jpeg, int x, int y)
{
    if (y >= jpeg.height || x >= jpeg.width)
        return NULL;

    return &(jpeg.pixels[y * jpeg.row_size + x * 3]);
}