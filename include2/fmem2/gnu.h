#ifndef FMEM2_GNU_H
#define FMEM2_GNU_H

#include <stdio.h>

#ifndef _SSIZE_T_DEFINED
typedef long ssize_t;
#define _SSIZE_T_DEFINED
#endif

#ifndef _OFF64_T_DEFINED
typedef int64_t off64_t;
#define _OFF64_T_DEFINED
#endif

/**
 *
 */
typedef ssize_t fmem2_cookie_read_function_t(void *cookie, char *buf, int size);

/**
 *
 */
typedef ssize_t fmem2_cookie_write_function_t(void *cookie, const char *buf, int size);

/**
 *
 */
typedef int fmem2_cookie_seek_function_t(void *cookie, off64_t *offset, int whence);

/**
 *
 */
typedef int fmem2_cookie_close_function_t(void *cookie);

/**
 *
 */
typedef struct {
    fmem2_cookie_read_function_t *read;
    fmem2_cookie_write_function_t *write;
    fmem2_cookie_seek_function_t *seek;
    fmem2_cookie_close_function_t *close;
} fmem2_cookie_io_functions_t;

/**
 *
 */
FILE *fmem2_fopencookie(void *cookie, const char *mode, fmem2_cookie_io_functions_t io_funcs);

#endif // FMEM2_GNU_H