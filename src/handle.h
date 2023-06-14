#ifndef _PEL_HANDLE_H_INCLUDED
#define _PEL_HANDLE_H_INCLUDED

#include "pel.h"
#include "image.h"

typedef struct _pel_handle {
    char* _fn_in;
    char* _fn_out;
    
    pel_image_type _image_in_type;
    pel_image_type _image_out_type;

    void* _in_image_struct;
    void* _out_image_struct;

    pel_color_t** pixels;

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
