#ifndef _PEL_WEBP_EASY_H_INCLDUED
#define _PEL_WEBP_EASY_H_INCLDUED

#include <stdint.h>
#include <webp/encode.h>

#include "pel.h"

typedef struct _webp_easy_webp {
    int width;
    int height;

    int stride;

    uint8_t* pixels;
} webp_easy_webp_t;

int _webp_easy_read(char* filename, webp_easy_webp_t* webp);

int _webp_easy_create_empty(char* filename, int width, int height);

int _webp_easy_write(char* filename, webp_easy_webp_t webp);

typedef void (*_webp_easy_draw_cb)(int x, int y, uint8_t* px);

int _webp_easy_draw(webp_easy_webp_t webp, _webp_easy_draw_cb draw_cb, pel_cord_t rect_start, pel_cord_t rect_end);

uint8_t* _webp_easy_px(webp_easy_webp_t webp, int x, int y);

#endif