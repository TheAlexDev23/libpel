#ifndef _PEL_LINE_H_INCLUDED
#define _PEL_LINE_H_INCLUDED

#include "pel.h"

void get_line_dxdy(pel_cord_t start, pel_cord_t end, int* dx, int *dy);
void line_eq(pel_cord_t start, pel_cord_t end, float* m, float* b);

#endif