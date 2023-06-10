#ifndef _PEL_ERROR_H_INCLUDED
#define _PEL_ERROR_H_INCLUDED

typedef int PEL_ERROR;

#define PEL_ERR_UNKOWN -1
#define PEL_ERR_HANDLE_NULL 1
#define PEL_ERR_PNG_EASY 2
#define PEL_ERR_FT_EASY 3

/* Returns a pointer to a string with a description of the current error */
char* pel_strerrno();
/* Returns a pointer to a string with a description of the error */
char* pel_strerr(PEL_ERROR err);
/* Returns the current error number */
PEL_ERROR pel_errno();

#endif