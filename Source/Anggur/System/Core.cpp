#include <glad/glad.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <Anggur/Helper/Log.hpp>
#include "Core.hpp"

namespace Anggur {

void Core::Initialize()
{
    static bool isInitialized = false;

    if (!isInitialized)
    {
        int result = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
        Anggur_Assert(result == 0, "[Core] Failed to initialize SDL: %i\n", result);

        isInitialized = true;
    }
}

void Core::Terminate()
{
    Mix_Quit();
    SDL_Quit();
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

    // Anggur_Log("[Core] Loaded OpenGl functions\n");
}

}
