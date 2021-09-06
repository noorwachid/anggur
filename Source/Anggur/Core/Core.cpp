#include <SDL.h>
#include <SDL_events.h>
#include <SDL_timer.h>
#include <glad/glad.h>
#include <Anggur/Helper/Log.hpp>
#include <Anggur/Renderer/Renderer.hpp>
#include "Core.hpp"
#include "Input.hpp"

namespace Anggur {

WindowConfig Core::windowConfig;
Window* Core::window = nullptr;
Scene* Core::scene = nullptr;
float Core::deltaTime = 0;
float Core::elapsedTime = 0;

Core::Core()
{
}

Window* Core::GetWindow()
{
    return window;
}

void Core::SetScene(Scene* activeScene)
{
    if (scene)
    {
        scene->Destroy();
        delete scene;
        scene = nullptr;
    }
    scene = activeScene;
    if (scene)
    {
        scene->Create();
        scene->Start();
    }
}

float Core::GetDeltaTime()
{
    return deltaTime;
}

float Core::GetElapsedTime()
{
    return elapsedTime;
}

void Core::ProcessEvent(SDL_Event* event)
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
                    break;
                }
                case SDL_WINDOWEVENT_RESIZED:
                {
                    break;
                }
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            KeyEvent e;
            e.type = EventType::KeyPressed;
            e.scancode = static_cast<Scancode>(event->key.keysym.scancode),
            e.key = static_cast<Key>(event->key.keysym.sym),
            e.modifier = static_cast<Modifier>(event->key.keysym.mod);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_KEYUP:
        {
            KeyEvent e;
            e.type = EventType::KeyReleased;
            e.scancode = static_cast<Scancode>(event->key.keysym.scancode),
            e.key = static_cast<Key>(event->key.keysym.sym),
            e.modifier = static_cast<Modifier>(event->key.keysym.mod);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            MouseScrollEvent e;
            e.type = EventType::MouseScrolled;
            e.group = EventGroup::Mouse;
            e.direction.Set(event->wheel.x, event->wheel.y);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            MouseMovementEvent e;
            e.type = EventType::MouseMoved;
            e.group = EventGroup::Mouse;
            e.position.Set(event->motion.x, event->motion.y);
            e.deltaPosition.Set(event->motion.xrel, event->motion.yrel);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            MouseButtonEvent e;
            e.type = EventType::MouseButtonPressed;
            e.group = EventGroup::Mouse;
            e.position.Set(event->button.x, event->button.y);
            e.button = static_cast<MouseButton>(event->button.button);
            if (scene) scene->OnEvent(e);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            MouseButtonEvent e;
            e.type = EventType::MouseButtonReleased;
            e.group = EventGroup::Mouse;
            e.position.Set(event->button.x, event->button.y);
            e.button = static_cast<MouseButton>(event->button.button);
            if (scene) scene->OnEvent(e);
            break;
        }
        default: break;
    }
}

void Core::Run(Scene* mainScene)
{
    Core::Initialize();

    window = new Window(windowConfig);
    Input::windowHandler = window->handler;

    Renderer::Initialize();
    Input::Initialize();

    SDL_Event event;
    ulong prevTimePoint = SDL_GetPerformanceCounter();

    scene = mainScene;
    if (scene)
    {
        scene->Create();
        scene->Start();
    }

    while (window->IsOpen())
    {
        Input::PreUpdate();

        while (SDL_PollEvent(&event))
            ProcessEvent(&event);

        ulong currTimePoint = SDL_GetPerformanceCounter();
        deltaTime = (currTimePoint - prevTimePoint) / static_cast<float>(SDL_GetPerformanceFrequency());
        elapsedTime += deltaTime;
        prevTimePoint = currTimePoint;

        Input::Update();
        if (scene)
        {
            scene->Update();
        }

        window->SwapBuffers();
    }


    if (scene)
    {
        scene->Destroy();
        delete scene;
        scene = nullptr;
    }

    Renderer::Terminate();
    Terminate();

}

void Core::SetWindowConfig(const WindowConfig& config)
{
    windowConfig = config;
}

void Core::SetVsync(bool enable)
{
    SDL_GL_SetSwapInterval(enable);
}

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

void Core::LoadGraphicsFunctions()
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
