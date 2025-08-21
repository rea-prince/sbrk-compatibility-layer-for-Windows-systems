#ifndef SBRK_COMPAT_H
#define SBRK_COMPAT_H

// for size_t, intptr_t
#include <stddef.h>

// for windows
#ifdef _WIN32
#include <windows.h>

static inline void* sbrk(intptr_t increment) {
    static char* heap_start = NULL;
    static char* heap_end = NULL;
    static size_t heap_size = 0;

    if (heap_start == NULL) {
        heap_size = 1024 * 1024;
        heap_start = (char*) VirtualAlloc(NULL, heap_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (heap_start == NULL) {
            return (void*)-1;
        }
        heap_end = heap_start;
    }

    if (heap_end + increment > heap_start + heap_size) {
        return (void*)-1;
    }

    void *prev_heap_end = heap_end;
    heap_end += increment;
    return prev_heap_end;
}

#else
#include <unistd.h> // on Unix, just use the real one
#endif

#endif // SBRK_COMPAT_H
