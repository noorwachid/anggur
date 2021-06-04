#include <SDL_events.h>
#include <SDL_timer.h>
#include <Anggur/Helper/Log.hpp>
#include "Core.hpp"
#include "Application.hpp"
#include "Input.hpp"

namespace Anggur {

Application* Application::instance = nullptr;

Application::Application()
{
    instance = this;
}

Window& Application::GetWindow()
{
    return *window;
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
                    OnEvent(e);
                    break;
                }
                case SDL_WINDOWEVENT_RESIZED:
                {
                    Renderer::SetViewport(Vector(event->window.data1, event->window.data2));
                    WindowEvent e(EventType::WindowResized);
                    e.size.Set(event->window.data1, event->window.data2);
                    OnEvent(e);
                    break;
                }
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            KeyEvent e(EventType::KeyPressed,
                       event->key.keysym.scancode,
                       static_cast<Key>(event->key.keysym.sym),
                       static_cast<KeyMod>(event->key.keysym.mod));
            OnEvent(e);
            break;
        }
        case SDL_KEYUP:
        {
            KeyEvent e(EventType::KeyReleased,
                       event->key.keysym.scancode,
                       static_cast<Key>(event->key.keysym.sym),
                       static_cast<KeyMod>(event->key.keysym.mod));
            OnEvent(e);
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            Input::mouseWheel.Set(event->wheel.x, event->wheel.y);
            MouseEvent e(EventType::MouseScrolled);
            e.wheel.Set(event->wheel.x, event->wheel.y);
            OnEvent(e);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            MouseEvent e(EventType::MouseMoved);
            e.pos.Set(event->motion.x, event->motion.y);
            e.dx.Set(event->motion.xrel, event->motion.yrel);
            OnEvent(e);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            MouseEvent e(EventType::MousePressed);
            e.pos.Set(event->button.x, event->button.y);
            e.button = static_cast<Mouse>(event->button.button);
            OnEvent(e);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            MouseEvent e(EventType::MouseReleased);
            e.pos.Set(event->button.x, event->button.y);
            e.button = static_cast<Mouse>(event->button.button);
            OnEvent(e);
            break;
        }
        default: break;
    }
}

void Application::OnCreate() {}
void Application::OnStart() {}
void Application::OnUpdate() {}
void Application::OnDestroy() {}

void Application::OnEvent(Event& event) {}

Application& Application::Get()
{
    return *instance;
}

void Application::Initialize()
{
    OnCreate();
    window = new Window(windowConfig);
    Input::windowHandler = window->handler;

    // Anggur_Log("[Core.Application] Initialized\n");
}

void Application::Run()
{
    Initialize();
    Renderer::Initialize();
    Audio::Initialize();
    Input::Initialize();

    SDL_Event event;
    Uint64 prevTimePoint = SDL_GetPerformanceCounter();
    Timer::elapsed = 0;

    OnStart();

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
        OnUpdate();

        window->SwapBuffers();
    }

    Terminate();
}

void Application::Terminate()
{
    OnDestroy();
    Audio::Terminate();
    Renderer::Terminate();
    Core::Terminate();

    // Anggur_Log("[Core.Application] Terminated\n");
}

}
