#ifndef _PEL_STATE_H_INLDUED
#define _PEL_STATE_H_INLDUED

#include "handle.h"

#define CHECK if (_pel_get_cur_handle() == NULL) return -1;

#endif