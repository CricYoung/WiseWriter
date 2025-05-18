// memtrace.hpp — Minimalistic global memory tracker
#pragma once

#ifdef MEMTRACE_ENABLED

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>

// Record info
struct MemRecord {
    const char* file;
    int line;
    size_t size;
};

// Very simple registry (fixed-size, no STL)
constexpr int MAX_RECORDS = 10000;
static void* g_ptrs[MAX_RECORDS];
static MemRecord g_records[MAX_RECORDS];
static int g_count = 0;

inline void add_record(void* ptr, size_t size, const char* file, int line) {
    if (g_count < MAX_RECORDS) {
        g_ptrs[g_count] = ptr;
        g_records[g_count] = {file, line, size};
        ++g_count;
    }
}

inline void remove_record(void* ptr) {
  for (int i = g_count - 1; i >= 0; --i)  {
        if (g_ptrs[i] == ptr) {
            g_ptrs[i] = g_ptrs[g_count - 1];
            g_records[i] = g_records[g_count - 1];
            --g_count;
            return;
        }
    }
}

// Custom new/delete operators
inline void* operator new(size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr) add_record(ptr, size, file, line);
    return ptr;
}

inline void* operator new[](size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr) add_record(ptr, size, file, line);
    return ptr;
}

inline void operator delete(void* ptr) noexcept {
    remove_record(ptr);
    free(ptr);
}

inline void operator delete[](void* ptr) noexcept {
    remove_record(ptr);
    free(ptr);
}

// Fallbacks
inline void* operator new(size_t size) {
    return operator new(size, "[unknown]", 0);
}

inline void* operator new[](size_t size) {
    return operator new[](size, "[unknown]", 0);
}

inline void operator delete(void* ptr, size_t) noexcept {
    operator delete(ptr);
}

inline void operator delete[](void* ptr, size_t) noexcept {
    operator delete[](ptr);
}

// Leak report
inline void memtrace_report() {
    if (g_count == 0) {
        fprintf(stderr, "[✓] No memory leaks detected.\n");
    } else {
        fprintf(stderr, "[!] Memory leaks detected: %d entries\n", g_count);
        for (int i = 0; i < g_count; ++i) {
            fprintf(stderr, "  Leak at %p, size: %zu, allocated at %s:%d\n",
                    g_ptrs[i], g_records[i].size, g_records[i].file, g_records[i].line);
        }
    }
}

#define new new(__FILE__, __LINE__)

#endif // MEMTRACE_ENABLED
