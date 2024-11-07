#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <fmem2.h>

#define release_assert(x) if (!(x)) { fputs("assertion failed at " __FILE__ ":" #__LINE__ ": " #x "\n", stderr); abort(); }

void test_hello_world() {
    char const expected[] = "Hello world!\n";
    size_t const expected_len = (sizeof(expected) / sizeof(expected[0])) - 1;
    FILE *const readable = fmem2_fmemopen((void *)expected, expected_len, "r");
    release_assert(!errno);
    errno = 0;
    release_assert(readable);

    char *actual;
    size_t actual_len;
    FILE *const writable = fmem2_open_memstream(&actual, &actual_len);
    release_assert(!errno);
    errno = 0;
    release_assert(writable);

    char chunk[1024];
    size_t chunk_len = sizeof(chunk) / sizeof(chunk[0]);
    size_t read_len;
    while (true) {
        read_len = fread(chunk, sizeof(chunk[0]), chunk_len, readable);
        release_assert(!errno);
        errno = 0;
        if (read_len == 0) {
            break;
        }
        fwrite(chunk, sizeof(chunk[0]), read_len, writable);
        release_assert(!errno);
        errno = 0;
    }
    fclose(readable);
    release_assert(!errno);
    errno = 0;
    fclose(writable);
    release_assert(!errno);
    errno = 0;

    release_assert(actual_len == expected_len);
    release_assert(memcmp(actual, expected, actual_len) == 0);

    free(actual);
    release_assert(!errno);
    errno = 0;
}

int main() {
    test_hello_world();
    release_assert(!errno);
    errno = 0;

    return 0;
}