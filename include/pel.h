#ifndef _PEL_INCLUDED
#define _PEL_INCLUDED

/* Initializes png file */
extern int pel_init(char* filename, int height, int width);
/* 
* Draws circle of radius r with it's center at image center + (x, y) to filename 
* Filename needs to be valid png otherwise will return -1
*/
extern int pel_draw_circle(char* filename, int x, int y, int r);

#endif