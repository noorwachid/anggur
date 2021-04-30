#pragma once

#ifdef NDEBUG
    #define Anggur_Log(...)
    #define Anggur_Assert(x, ...)
#else
    #include <stdio.h>
    #include <assert.h>
    #define Anggur_Log(...) ::printf(__VA_ARGS__);
    #define Anggur_Assert(x, ...) \
        if (!(x)) { \
            ::printf(__VA_ARGS__); \
        }
#endif
