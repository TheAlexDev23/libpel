#ifndef _PNG_EASY_INCLUDED
#define _PNG_EASY_INCLUDED

#include <png.h>

/* Reads file filename writes to row_pointers if succesful */
int png_easy_read(char* filename, png_bytep** row_pointers);

/* Writes row_pointers into filename if succesful */
int png_easy_write(char* filename, png_bytep* row_pointers);

#endif