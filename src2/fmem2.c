#include <fmem2/posix.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <wchar.h>

FILE *fmem2_fmemopen(void *buf, size_t size, const char *mode) {
    return fmemopen(buf, size, mode);
}

FILE *fmem2_open_memstream(char **ptr, size_t *sizeloc) {
    return open_memstream(ptr, sizeloc);
}

FILE *fmem2_open_wmemstream(wchar_t **ptr, size_t *sizeloc) {
    return open_wmemstream(ptr, sizeloc);
}

#elif _WIN32

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <io.h>

#define todo() do { fputs("todo at " __FILE__ ":" #__LINE__ "\n", stderr); abort(); } while (0);

// The trick here is to use a temporary file (which may be in memory) and then watch it for changes.

FILE *fmem2_fmemopen(void *buf, size_t size, const char *mode) {
    // 1. Create a temporary file with the special temporary attribute and the delete-on-close attribute.
    // 2. Obtain a FILE * handle to the file.
    // 3. Register a callback to watch the file for changes. Each time the file changes, update the memory.
}

FILE *fmem2_open_memstream(char **ptr, size_t *sizeloc) {
    todo();
}

FILE *fmem2_open_wmemstream(wchar_t **ptr, size_t *sizeloc) {
    todo();
}

#else

#include <stdio.h>
#include <errno.h>

FILE *fmem2_fmemopen(void *buf, size_t size, const char *mode) {
    errno = /* ENOSYS */ 38;
    return NULL;
}

FILE *fmem2_open_memstream(char **ptr, size_t *sizeloc) {
    errno = /* ENOSYS */ 38;
    return NULL;
}

FILE *fmem2_open_wmemstream(wchar_t **ptr, size_t *sizeloc) {
    errno = /* ENOSYS */ 38;
    return NULL;
}

#endif
