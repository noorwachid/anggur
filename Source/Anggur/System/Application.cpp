#include <SDL_events.h>
#include <SDL_timer.h>
#include <Anggur/Helper/Log.hpp>
#include "Core.hpp"
#include "Application.hpp"
#include "Input.hpp"

namespace Anggur {

WindowConfig Application::windowConfig;
Window* Application::window = nullptr;
Scene* Application::scene = nullptr;

Application::Application()
{
}

Window* Application::GetWindow()
{
    return window;
}

void Application::SetScene(Scene* activeScene)
{
    if (scene)
    {
        scene->OnDetach();
        delete scene;
        scene = nullptr;
    }
    scene = activeScene;
    if (scene) scene->OnAttach();
}

void Application::ProcessEvent(SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_QUIT:
        {
            window->Close();
            break;
        }
        case SDL_WINDOWEVENT:
        {
            switch (event->window.event)
            {
                case SDL_WINDOWEVENT_MOVED:
                {
                    WindowEvent e(EventType::WindowMoved);
                    e.pos.Set(event->window.data1, event->window.data2);
                    if (scene) scene->OnEvent(e);
                    break;
                }
                case SDL_WINDOWEVENT_RESIZED:
                {
                    Renderer::SetViewport(Vector(event->window.data1, event->window.data2));
                    WindowEvent e(EventType::WindowResized);
                    e.size.Set(event->window.data1, event->window.data2);
                    if (scene) scene->OnEvent(e);
                    break;
                }
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            KeyEvent e(EventType::KeyPressed,
                       static_cast<Key>(event->key.keysym.scancode),
                       static_cast<VirKey>(event->key.keysym.sym),
                       static_cast<ModKey>(event->key.keysym.mod));
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_KEYUP:
        {
            KeyEvent e(EventType::KeyReleased,
                       static_cast<Key>(event->key.keysym.scancode),
                       static_cast<VirKey>(event->key.keysym.sym),
                       static_cast<ModKey>(event->key.keysym.mod));
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            Input::mouseWheel.Set(event->wheel.x, event->wheel.y);
            MouseEvent e(EventType::MouseScrolled);
            e.wheel.Set(event->wheel.x, event->wheel.y);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            MouseEvent e(EventType::MouseMoved);
            e.pos.Set(event->motion.x, event->motion.y);
            e.dx.Set(event->motion.xrel, event->motion.yrel);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            MouseEvent e(EventType::MousePressed);
            e.pos.Set(event->button.x, event->button.y);
            e.button = static_cast<Mouse>(event->button.button);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            MouseEvent e(EventType::MouseReleased);
            e.pos.Set(event->button.x, event->button.y);
            e.button = static_cast<Mouse>(event->button.button);
            if (scene) scene->OnEvent(e);
            break;
        }
        default: break;
    }
}

void Application::Run(Scene* mainScene)
{
    window = new Window(windowConfig);
    Input::windowHandler = window->handler;

    Renderer::Initialize();
    Audio::Initialize();
    Input::Initialize();

    SDL_Event event;
    Uint64 prevTimePoint = SDL_GetPerformanceCounter();
    Timer::elapsed = 0;

    scene = mainScene;
    if (scene) scene->OnAttach();

    while (window->IsOpen())
    {
        Input::PreUpdate();

        while (SDL_PollEvent(&event))
            ProcessEvent(&event);

        Uint64 currTimePoint = SDL_GetPerformanceCounter();
        Timer::delta = (currTimePoint - prevTimePoint) / static_cast<float>(SDL_GetPerformanceFrequency());
        Timer::elapsed += Timer::delta;
        prevTimePoint = currTimePoint;

        Input::Update();
        if (scene) scene->OnUpdate();

        window->SwapBuffers();
    }


    if (scene)
    {
        scene->OnDetach();
        delete scene;
        scene = nullptr;
    }

    Audio::Terminate();
    Renderer::Terminate();
    Core::Terminate();

}

void Application::SetWindowConfig(const WindowConfig& config)
{
    windowConfig = config;
}

void Application::SetVsync(bool enable)
{
    SDL_GL_SetSwapInterval(enable);
}

}
