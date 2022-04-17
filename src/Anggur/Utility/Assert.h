#pragma once

#ifdef NDEBUG
#define ANGGUR_ASSERT(x, ...)
#else
#include <cstdio>
#define ANGGUR_ASSERT(x, ...)  \
    if (!(x))                  \
    {                          \
        ::printf(__VA_ARGS__); \
        ::printf("\n");        \
        ::exit(1);             \
    }
#endif
