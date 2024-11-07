#ifndef FMEM2_POSIX_H
#define FMEM2_POSIX_H

#include <stdio.h>
#include <wchar.h>

/**
 *
 */
FILE *fmem2_fmemopen(void *buf, size_t size, const char *mode);

/**
 *
 */
FILE *fmem2_open_memstream(char **ptr, size_t *sizeloc);

/**
 *
 */
FILE *fmem2_open_wmemstream(wchar_t **ptr, size_t *sizeloc);

#endif // FMEM2_POSIX_H