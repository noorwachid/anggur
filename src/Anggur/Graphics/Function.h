#pragma once

#include "glad/glad.h"
#include "Anggur/Utility/Log.h"

#ifndef NDEBUG
    #define ANGGUR_GF(funcCall) \
        while (glGetError() != GL_NO_ERROR); \
        funcCall; \
        while (GLenum error = glGetError()) { \
            ANGGUR_LOG("[Graphic.Function] Error (0x%x):", error); \
            ANGGUR_LOG("[Graphic.Function]     in function %s", #funcCall); \
            ANGGUR_LOG("[Graphic.Function]     at %s:%i", __FILE__, __LINE__); \
            break; \
        } 
#else
    #define ANGGUR_GF(funcCall) funcCall
#endif

#include <glad/glad.h>