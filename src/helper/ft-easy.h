#ifndef _PEL_TT_EASY_H_INCLUDED
#define _PEL_TT_EASY_H_INCLUDED

#include "pel.h"

/* Sets font bitmap of charcter from default location. Doesn't work too well on all platforms */
int _ft_easy_get_bm(char* fontname, char character, pel_bitmap_t* bitmap);

/* Sets font bitmap of character from custom location filename */
int _ft_easy_get_bm_loc(char* filename, char character, pel_bitmap_t* bitmap);

#endif