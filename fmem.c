/*!
MIT License

Copyright (c) 2017 Franklin "Snaipe" Mathieu <http://snai.pe/>
Copyright (c) 2024 Jacob Hummer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//region fmem.h
#ifndef FMEM_H_
#define FMEM_H_
/*!
MIT License

Copyright (c) 2017 Franklin "Snaipe" Mathieu <http://snai.pe/>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>

struct fmem_reserved {
    char reserved[32];
};

typedef struct fmem_reserved fmem;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes an fmem structure.
 *
 * This function prepares the fmem structure for use. It must be called
 * before any other fmem functions are used with this structure.
 *
 * @param file A pointer to an fmem structure.
 */
FMEM_API void fmem_init(fmem *file);

/**
 * Cleans up an fmem structure.
 *
 * This function releases any resources held by the fmem structure. 
 * It should be called when you are finished with the structure to prevent memory leaks.
 *
 * @param file A pointer to an fmem structure.
 */
FMEM_API void fmem_term(fmem *file);

/**
 * Opens a memory-backed file stream.
 *
 * This function attempts to open a memory-backed file stream, trying various
 * methods depending on platform support (e.g., open_memstream, fopencookie).
 * The 'mode' parameter is currently ignored, and streams are always opened
 * in write-only mode.
 *
 * @param file A pointer to an fmem structure.
 * @param mode A string representing the file mode (ignored in current implementation).
 * @return A FILE pointer to the memory-backed file stream, or NULL if the operation fails.
 */
FMEM_API FILE *fmem_open(fmem *file, const char *mode);

/**
 * Retrieves the memory buffer and size from an fmem structure.
 *
 * This function allows access to the underlying memory buffer used by the fmem structure.
 * It is useful for reading data written to the memory-backed file stream.
 *
 * @param file A pointer to an fmem structure.
 * @param mem A pointer to a pointer where the address of the memory buffer will be stored.
 * @param size A pointer to a size_t where the size of the buffer will be stored.
 */
FMEM_API void fmem_mem(fmem *file, void **mem, size_t *size);

#ifdef __cplusplus
}
#endif

#endif // FMEM_H_
//endregion
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#if WIN32

#elif 0

#else

typedef struct {
    FILE *file;
    void *buf;
} fmem_impl;

typedef union {
    fmem *fm;
    fmem_impl *impl;
} fmem_conv;

void fmem_init(fmem *file) {
    fmem_conv cv = { .fm = file };
    memset(cv.impl, 0, sizeof (*cv.impl));
}

void fmem_term(fmem *file) {
    fmem_conv cv = { .fm = file };

    if (cv.impl->buf) {
        free(cv.impl->buf);
    }
}

FILE *fmem_open(fmem *file, const char *mode) {
    fmem_conv cv = { .fm = file };
    FILE *f = tmpfile();
    if (f) {
        if (cv.impl->buf) {
            free(cv.impl->buf);
        }
        cv.impl->file = f;
    }
    return f;
}

void fmem_mem(fmem *file, void **mem, size_t *size) {
    fmem_conv cv = { .fm = file };
    *mem = NULL;
    *size = 0;

    if (cv.impl->buf) {
        free(cv.impl->buf);
        cv.impl->buf = NULL;
    }

    fseek(cv.impl->file, 0, SEEK_END);
    long bufsize = ftell(cv.impl->file);
    if (bufsize < 0) {
        return;
    }
    
    void *buf = malloc(bufsize);
    if (!buf) {
        return;
    }

    rewind(cv.impl->file);
    if (fread(buf, 1, bufsize, cv.impl->file) != (size_t) bufsize) {
        free(buf);
        return;
    }
    cv.impl->buf = buf;
    *mem = buf;
    *size = bufsize;
}

#endif