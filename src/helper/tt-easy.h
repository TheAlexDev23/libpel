#ifndef _PEL_TT_EASY_H_INCLUDED
#define _PEL_TT_EASY_H_INCLUDED

#include "pel.h"

/* Sets font bitmap from default location */
int _tt_easy_get_bm(char* fontname, pel_bitmap_t* bitmap);

/* Sets font bitmap from custom location filename */
int _tt_easy_get_bm_loc(char* filename, pel_bitmap_t* bitmap);

#endif