#ifndef FMEM2_BSD_H
#define FMEM2_BSD_H

#include <stdio.h>

/**
 *
 */
FILE *fmem2_funopen(void const *cookie, int (*readfn)(void *, char *, int), int (*writefn)(void *, const char *, int), fpos_t (*seekfn)(void *, fpos_t, int), int (*closefn)(void *));

/**
 *
 */
FILE *fmem2_fropen(void *cookie, int (*readfn)(void *, char *, int));

/**
 *
 */
FILE *fmem2_fwopen(void *cookie, int (*writefn)(void *, const char *, int));

#endif // FMEM2_BSD_H