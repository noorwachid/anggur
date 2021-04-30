#include <Anggur/Math/Vector.hpp>
#include <Anggur/Renderer/Image.hpp>
#include <Anggur/Helper/Io.hpp>
#include <Anggur/Helper/Log.hpp>
#include "Window.hpp"

namespace Anggur {

Window::Window(const WindowConfig& config)
{
    Core::Initialize();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    int flags = SDL_WINDOW_OPENGL;
    flags |= SDL_WINDOW_RESIZABLE;
    //flags |= SDL_WINDOW_MAXIMIZED;

    mRawWindow  = SDL_CreateWindow(config.title.c_str(),
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                config.width, config.height,
                                flags);
    mContext = SDL_GL_CreateContext(mRawWindow);
    mRatio   = config.width / static_cast<float>(config.height);
    mOpen    = true;
    SDL_GL_MakeCurrent(mRawWindow, mContext);

    Core::LoadGlFunctions();
}

Window* Window::Create(const WindowConfig& config)
{
    return new Window(config);
}

float Window::GetRatio()
{
    return mRatio;
}

WindowSize Window::GetSize()
{
    int w, h;
    SDL_GetWindowSize(mRawWindow, &w, &h);
    return WindowSize(w, h);
}

bool Window::IsOpen()
{
    return mOpen;
}

void Window::SwapBuffers()
{
    SDL_GL_SwapWindow(mRawWindow);
}

SDL_Window* Window::GetRawWindow()
{
    return mRawWindow;
}

SDL_GLContext Window::GetContext()
{
    return mContext;
}

void Window::Close()
{
    mOpen = false;
}

}
