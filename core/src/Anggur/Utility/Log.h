#pragma once

#ifdef NDEBUG
#define ANGGUR_LOG(...)
#else
#include <cstdio>
#define ANGGUR_LOG(...) ::printf(__VA_ARGS__); ::printf("\n");
#endif
