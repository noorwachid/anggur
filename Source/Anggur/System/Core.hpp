#pragma once

#include "glad/glad.h"
#include "SDL.h"

namespace Anggur {

class Core
{
public:
    static void Initialize();
    static void Terminate();
    static void SetVsync(bool enable);

    static void LoadGlFunctions();
};

}

