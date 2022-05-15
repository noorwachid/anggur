#pragma once

#ifdef __linux__
    #define ANGGUR_OS_LINUX
#endif

#ifdef __APPLE__
	#define ANGGUR_OS_APPLE
#endif

#ifdef _WIN32 || _WIN64
    #define ANGGUR_PLATFORM_DESKTOP
    #define ANGGUR_OS_WINDOWS
    #ifdef _WIN32
        #define ANGGUR_ARCH_32
    #endif
    #ifdef _WIN64
        #define ANGGUR_ARCH_64
    #endif
#endif

#ifdef __ANDROID__
    #define ANGGUR_PLATFORM_MOBILE
    #define ANGGUR_OS_ANDROID
#endif

#ifdef __EMSCRIPTEN
    #define ANGGUR_PLATFORM_WEB
#endif