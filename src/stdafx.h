#pragma once
#include <cstddef>
#include <cstdlib>

// NOTE(Cade):
//   This is a dummy file which multiple external files will include.
//   It also contains numerous hacks to minimize patches in the advancedfx repo(s).
//
//   Most of these functions are "unused"; The templates using them aren't instantiated.
//   I suspect MSVC didn't link them as a result. But GCC still tries.

#define COMPILE_TIME_ASSERT(x)
#define MEM_ALLOC_CREDIT_CLASS()
#define __cdecl

static inline bool IsDebug() { return false; }

template <class... T>
static inline void ReconnectInterface(T&&...) {}

template <class... T>
static inline void V_swap(T&&...) {}

static inline void DebuggerBreak() {}

static void* _aligned_malloc(size_t size, size_t align) {
    return std::aligned_alloc(align, size);
}

class CRawAllocator {
public:
    template <class... T>
    static void* Alloc(T&&...) { return nullptr; }

    template <class... T>
    static void Free(T&&...) { }
};
