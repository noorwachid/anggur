#include <SDL.h>
#include <SDL_events.h>
#include <SDL_timer.h>
#include <glad/glad.h>
#include <Anggur/Helper/Log.h>
#include <Anggur/Renderer/Renderer.h>
#include "Application.h"
#include "Input.h"

namespace Anggur {

WindowConfig Application::windowConfig;
Window* Application::window = nullptr;
Scene* Application::scene = nullptr;
float Application::deltaTime = 0;
float Application::elapsedTime = 0;

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

float Application::GetDeltaTime()
{
    return deltaTime;
}

float Application::GetElapsedTime()
{
    return elapsedTime;
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
            Input::wheel = e.direction;
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
            Input::cursor = e.position;
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

void Application::Run(Scene* mainScene)
{
    Application::Initialize();

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

void Application::SetWindowConfig(const WindowConfig& config)
{
    windowConfig = config;
}

void Application::SetVsync(bool enable)
{
    SDL_GL_SetSwapInterval(enable);
}

void Application::Initialize()
{
    static bool isInitialized = false;

    if (!isInitialized)
    {
        // Do not disable compositing
        SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
        
        int result = SDL_Init(SDL_INIT_VIDEO);
        ANGGUR_ASSERT(result == 0, "[Core] Failed to initialize SDL: %i\n", result);

        isInitialized = true;
    }
}

void Application::Terminate()
{
    SDL_Quit();
}

void Application::LoadGraphicsFunctions()
{
    static bool isInitialized = false;

    if (!isInitialized)
    {
        int result = gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
        ANGGUR_ASSERT(result != 0, "[Core] Failed to load OpenGl functions");
        isInitialized = true;
    }

    // Anggur_Log("[Core] Loaded OpenGl functions\n");
}

}
