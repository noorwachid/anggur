#pragma once

#ifdef NDEBUG
#define ANGGUR_LOG(...)
#define ANGGUR_ASSERT(x, ...)
#else
#include <assert.h>
#include <stdio.h>
#define ANGGUR_LOG(...) ::printf(__VA_ARGS__);
#define ANGGUR_ASSERT(x, ...)  \
    if (!(x))                  \
    {                          \
        ::printf(__VA_ARGS__); \
        ::exit(1);             \
    }
#endif
