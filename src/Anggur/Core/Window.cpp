#include <SDL.h>
#include <Anggur/Math/Vector.h>
#include <Anggur/Renderer/Image.h>
#include <Anggur/Helper/IO.h>
#include <Anggur/Helper/Log.h>
#include "Window.h"
#include "Application.h"

namespace Anggur {

Window::Window(const WindowConfig& config)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    int flag = SDL_WINDOW_OPENGL | static_cast<int>(config.flag);

    handler  = SDL_CreateWindow(config.title.c_str(),
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                config.width, config.height,
                                flag);
    context = SDL_GL_CreateContext(handler);
    ratio   = config.width / static_cast<float>(config.height);
    isOpen    = true;
    SDL_GL_MakeCurrent(handler, context);

    Application::LoadGraphicsFunctions();
}

void Window::SetPosition(const Vector& pos)
{
    SDL_SetWindowPosition(handler, pos.x, pos.y);
}

void Window::SetSize(const Vector& size)
{
    SDL_SetWindowSize(handler, size.x, size.y);
}

void Window::SetTitle(const string& title)
{
    SDL_SetWindowTitle(handler, title.c_str());
}

float Window::GetRatio()
{
    return ratio;
}

Vector Window::GetPosition()
{
    int x, y;
    SDL_GetWindowPosition(handler, &x, &y);
    return Vector(x, y);
}

Vector Window::GetSize()
{
    int w, h;
    SDL_GetWindowSize(handler, &w, &h);
    return Vector(w, h);
}

const string& Window::GetTitle()
{
    title = SDL_GetWindowTitle(handler);
    return title;
}

Anggur::WindowFlag Window::GetFlag()
{
    return static_cast<WindowFlag>(SDL_GetWindowFlags(handler));
}

bool Window::IsOpen()
{
    return isOpen;
}

void Window::SwapBuffers()
{
    SDL_GL_SwapWindow(handler);
}

SDL_Window* Window::GetHandler()
{
    return handler;
}

void* Window::GetContext()
{
    return context;
}

void Window::Close()
{
    isOpen = false;
}

}
