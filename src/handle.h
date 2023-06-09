#ifndef _PEL_HANDLE_H_INCLUDED
#define _PEL_HANDLE_H_INCLUDED

#include "pel.h"

typedef struct _pel_handle {
    char* _fn;

    int _width;
    int _height;

    int _centerX;
    int _centerY;

    PEL_ERROR _err;
} pel_handle_t;

pel_handle_t* _pel_get_cur_handle();
void _pel_set_cur_handle(pel_handle_t* new_handle);
void _pel_free_cur_handle();

#endif