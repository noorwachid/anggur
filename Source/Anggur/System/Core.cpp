#include <glad/glad.h>
#include <SDL.h>
#include <Anggur/Helper/Log.hpp>
#include "Core.hpp"

namespace Anggur {

void Core::Initialize()
{

    static bool isInitialized = false;

    if (!isInitialized)
    {
        int result = SDL_Init(SDL_INIT_VIDEO);
        Anggur_Assert(result == 0, "[Core] Failed to initialize SDL: %i\n", result);

        isInitialized = true;
    }
}

void Core::Terminate()
{
    SDL_Quit();
}

void Core::SetVsync(bool enable)
{
    SDL_GL_SetSwapInterval(enable);
}

void Core::LoadGlFunctions()
{
    static bool isInitialized = false;

    if (!isInitialized)
    {
        int result = gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
        Anggur_Assert(result != 0, "[Core] Failed to load OpenGl functions");
        isInitialized = true;
    }

    Anggur_Log("[Core] Loaded OpenGl functions\n");
}

}
