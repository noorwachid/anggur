#include <SDL_events.h>
#include <SDL_timer.h>
#include <Anggur/Helper/Log.hpp>
#include "Core.hpp"
#include "Application.hpp"
#include "Input.hpp"

namespace Anggur {

Application* Application::mInstance = nullptr;

Application::Application()
{
    mInstance = this;
}

Window& Application::GetWindow()
{
    return *mWindow;
}

void Application::ProcessEvent(SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_QUIT:
        {
            mWindow->Close();
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
            Input::mMouseWheel.Set(event->wheel.x, event->wheel.y);
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

void Application::OnInitialize() {}
void Application::OnAttach() {}
void Application::OnUpdate(float dx) {}
void Application::OnDetach() {}
void Application::OnEvent(Event& event) {}

Application& Application::Get()
{
    return *mInstance;
}

void Application::Initialize()
{
    OnInitialize();
    mWindow = Window::Create(mWindowConfig);
    Input::mRawWindow = mWindow->mRawWindow;

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
    Timer::mElapsed = 0;

    OnAttach();


    while (mWindow->IsOpen())
    {
        Input::PreUpdate();

        while (SDL_PollEvent(&event))
            ProcessEvent(&event);

        Uint64 currTimePoint = SDL_GetPerformanceCounter();
        float dx = (currTimePoint - prevTimePoint) / static_cast<float>(SDL_GetPerformanceFrequency());
        prevTimePoint = currTimePoint;
        Timer::mDx = dx;
        Timer::mElapsed += dx;

        Input::Update();
        OnUpdate(dx);

        mWindow->SwapBuffers();
    }
    OnDetach();
    Audio::Terminate();
    Renderer::Terminate();
    Terminate();

    // Anggur_Log("[Core.Application] Terminated\n");
}

void Application::Terminate()
{
    Core::Terminate();
}

}
