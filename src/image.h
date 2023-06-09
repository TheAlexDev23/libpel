#ifndef _PEL_IMAGE_H_INCLDUED
#define _PEL_IMAGE_H_INCLDUED

/* Makes x,y = center + x,y on an image grid (starts top left) */
#define get_xy_rel_img_center(x, y, handle) x = handle->_centerX + x; y = handle->_centerY - y;

/* Makes x,y = center + x,y on an mathematical grid  */
#define get_xy_rel_grid_center(x, y, handle) (x = x - handle->_centerX) * -1; y = (y - handle->_centerY) * -1;

#endif