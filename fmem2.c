/**
 * MIT License
 * 
 * Copyright (c) 2024 Jacob Hummer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "fmem.h"

// POSIX

// Able to use native POSIX functions?
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

// Already defined by the included C standard library. No need for code here.

// Able to use native Windows functions?
#elif _WIN32

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <windows.h>
#include <io.h>

/**
 * 1. Create a temporary file using the Windows API
 * 2. Use FILE_ATTRIBUTE_TEMPORARY to indicate that the file is temporary
 * 3. Use FILE_FLAG_DELETE_ON_CLOSE to delete the file when it is closed
 */
FILE *fmemopen(void *buf, size_t size, const char *mode) {
    // Create a temporary file
    char temp_path[MAX_PATH];
    GetTempPathA(MAX_PATH, temp_path);
    char temp_file[MAX_PATH];
    GetTempFileNameA(temp_path, "fmem", 0, temp_file);
    HANDLE file = CreateFileA(temp_file, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        errno = GetLastError();
        return NULL;
    }

    // Write the buffer to the file
    DWORD bytes_written;
    if (!WriteFile(file, buf, size, &bytes_written, NULL)) {
        CloseHandle(file);
        errno = GetLastError();
        return NULL;
    }

    // Open the file with the specified mode
    int fd = _open_osfhandle((intptr_t)file, _O_RDWR);
    if (fd == -1) {
        CloseHandle(file);
        errno = GetLastError();
        return NULL;
    }
    FILE *stream = _fdopen(fd, mode);
    if (stream == NULL) {
        _close(fd);
        return NULL;
    }
    return stream;
}

// Fall back to temporary files.
#else

#endif

// GNU

// Able to use native GNU functions?
#if defined(__GLIBC__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)

// Able to use Windows functions?
#elif _WIN32

// Fall back to temporary files.
#else

#endif

// BSD

// Able to use native BSD functions?
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)

// Able to use GNU functions?
#elif defined(__GLIBC__)

// Able to use Windows functions?
#elif _WIN32

// Fall back to temporary files.
#else

#endif
