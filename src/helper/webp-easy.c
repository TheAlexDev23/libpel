#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <webp/decode.h>
#include <webp/encode.h>

#include "webp-easy.h"
#include "handle.h"

int _webp_easy_create_empty(char* filename, int width, int height)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) return -1;

    uint8_t* input = calloc(1, width * height * 4);

    uint8_t* output = NULL;

    size_t size = WebPEncodeRGBA(input, width, height, width * 4, 90, &output);

    fwrite(output, 1, size, fp);

    fclose(fp);
    free(input);

    return 0;
}

int _webp_easy_read(char* filename, webp_easy_webp_t* webp)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) return -1;
    
    // TODO: Repplace stat for multiplatform function with support of larger files in _webp_easy_read
    struct stat st;
    if (stat(filename, &st)) return -1;

    size_t file_size = st.st_size;

    uint8_t* file_buffer = malloc(file_size);

    fread(file_buffer, 1, file_size, fp);
    
    int width;
    int height;

    webp->pixels = WebPDecodeRGBA(file_buffer, file_size, &width, &height);

    webp->width = width;
    webp->height = height;
    webp->stride = width * 4;

    free(file_buffer);
    fclose(fp);

    return 0;
}

int _webp_easy_write(char* filename, webp_easy_webp_t webp)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) return -1;

    uint8_t* output = NULL;

    size_t size = WebPEncodeRGBA(webp.pixels, webp.width, webp.height, webp.stride, 90, &output);

    fwrite(output, 1, size, fp);

    fclose(fp);

    return 0;
}

int _webp_easy_draw(webp_easy_webp_t webp, _webp_easy_draw_cb draw_cb, pel_cord_t rect_start, pel_cord_t rect_end)
{
    pel_handle_t* handle = _pel_get_cur_handle();
    if (handle == NULL) return -1;

    for (int y = rect_start._y; y < rect_end._y; y++) {
        for (int x = rect_start._x; x < rect_end._x; x++) {
            int rx = x, ry = y;
            get_xy_rel_img_center(rx, ry, handle);
            uint8_t* px = _webp_easy_px(webp, rx, ry);
            if (px != NULL) draw_cb(rx, ry, px);
        }
    }

    return 0;
}

uint8_t* _webp_easy_px(webp_easy_webp_t webp, int x, int y)
{
    if (x >= webp.width || y >= webp.height) return NULL;

    return &(webp.pixels[y * webp.stride + x * 4]);
}