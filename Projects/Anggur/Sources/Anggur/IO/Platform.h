#pragma once

#ifdef __linux__
#define ANGGUR_OS_LINUX
#endif

#ifdef __APPLE__
#define ANGGUR_OS_X
#endif

#ifdef _WIN32
#define ANGGUR_OS_WINDOWS
#define ANGGUR_PLATFORM_DESKTOP
#define ANGGUR_ARCH_32
#endif
#ifdef _WIN64
#define ANGGUR_OS_WINDOWS
#define ANGGUR_PLATFORM_DESKTOP
#define ANGGUR_ARCH_64
#endif

#ifdef __ANDROID__
#define ANGGUR_PLATFORM_MOBILE
#define ANGGUR_OS_ANDROID
#endif

#ifdef __EMSCRIPTEN
#define ANGGUR_PLATFORM_WEB
#endif