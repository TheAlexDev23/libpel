#ifndef _PEL_TT_EASY_H_INCLUDED
#define _PEL_TT_EASY_H_INCLUDED

#include "pel.h"

/* Initialize freetype */
int _ft_easy_init();

/* Closes freetype */
int _ft_easy_exit();

/* Gets font bitmap platform-independently */
int _ft_easy_get_bm(pel_font_t font, char character, pel_bitmap_t* bitmap);

/* Sets font bitmap of character from custom location file_name */
int _ft_easy_get_bm_loc(char* filename, int size, char character, pel_bitmap_t* bitmap);

#endif