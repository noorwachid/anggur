#pragma once

#ifdef __linux__
#define ANGGUR_PLATFORM_DESKTOP
#define ANGGUR_OS_LINUX
#endif

#ifdef __APPLE__
#define ANGGUR_PLATFORM_DESKTOP
#define ANGGUR_OS_X
#endif

#if defined(_WIN32)  || defined(_WIN64)
#define ANGGUR_PLATFORM_DESKTOP
#define ANGGUR_OS_WINDOWS
#endif

#ifdef __ANDROID__
#define ANGGUR_PLATFORM_MOBILE
#define ANGGUR_OS_ANDROID
#endif

#ifdef __EMSCRIPTEN
#define ANGGUR_PLATFORM_WEB
#endif
