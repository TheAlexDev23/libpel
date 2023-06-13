#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>

#include <turbojpeg.h>
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

    int quality = PEL_JPEG_EASY_QUALITY;

    unsigned char* jpeg_buffer = (unsigned char *)malloc(width * height * 4);

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
        return -1;
    
    jpeg_stdio_dest(&cinfo, fp);
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, FALSE);

    // Write empty scanlines
    JSAMPROW row_pointer[1];
    int row_stride = width * cinfo.input_components;
    unsigned char dummy_scanline[row_stride];
    memset(dummy_scanline, 0, row_stride);  // Fill with zeros

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
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return -1;

    // TODO: Replace stat for a multiplatform function capable of reading large files in _jpeg_easy_read

    struct stat st;
    if (stat(filename, &st))
        return -1;

    int file_size = st.st_size;

    // Contents of file
    uint8_t *file_buffer = (unsigned char *)malloc(file_size);
    fread(file_buffer, 1, file_size, fp);
    fclose(fp);

    tjhandle tj = tjInitDecompress();

    if (tjDecompressHeader2(tj, file_buffer, file_size, &(jpeg->width), &(jpeg->height), &(jpeg->subsamp)))
        return -1;

    // Decompressed image
    jpeg->pixels = (uint8_t *)malloc(jpeg->width * jpeg->height * 3);

    if (tjDecompress2(tj, file_buffer, file_size, jpeg->pixels, jpeg->width, 0, jpeg->height, TJPF_RGB, 0))
        return -1;

    return tjDestroy(tj);
}

int _jpeg_easy_write(char *filename, jpeg_easy_jpeg_t jpeg)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    int quality = PEL_JPEG_EASY_QUALITY;
    int width = jpeg.width;
    int height = jpeg.height;
    int subsamp = jpeg.subsamp;

    tjhandle tj = tjInitCompress();

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
        return -1;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    cinfo.image_width = width;
    cinfo.image_height = height;

    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    
    jpeg_stdio_dest(&cinfo, fp);
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, FALSE);

    // Write empty scanlines
    JSAMPROW row_pointer[1];
    int row_stride = width * cinfo.input_components;

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &(jpeg.pixels[cinfo.next_scanline * jpeg.width]);
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

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

    return &(jpeg.pixels[y * jpeg.width + x * 3]);
}