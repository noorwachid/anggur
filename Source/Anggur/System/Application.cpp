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

void Application::ProcessEvent(SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_QUIT:
        {
            mWindow->Close();
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            Input::mMouseWheel.Set(event.wheel.x, event.wheel.y);
        }

        default: break;
    }
}

void Application::OnInitialize() {}
void Application::OnAttach() {}
void Application::OnUpdate(float deltaTime) {}
void Application::OnDetach() {}
void Application::OnEvent(Event* event) {}

Application& Application::Get()
{
    return *mInstance;
}

void Application::Initialize()
{
    OnInitialize();
    mWindow = Window::Create(mWindowConfig);
    Input::mRawWindow = mWindow->mRawWindow;

    Anggur_Log("[Core.Application] Initialized\n");
}

void Application::Run()
{
    Initialize();
    Renderer::Initialize();
    Input::Initialize();
    OnAttach();

    Uint64 prevTimePoint = SDL_GetPerformanceCounter();

    while (mWindow->IsOpen())
    {
        SDL_Event event;

        Input::PreUpdate();

        while (SDL_PollEvent(&event))
            ProcessEvent(event);

        Uint64 currTimePoint = SDL_GetPerformanceCounter();
        float dx = (currTimePoint - prevTimePoint) / static_cast<float>(SDL_GetPerformanceFrequency());
        prevTimePoint = currTimePoint;

        Input::Update();
        OnUpdate(dx);

        mWindow->SwapBuffers();
    }
    OnDetach();
    Renderer::Terminate();
    Terminate();

    Anggur_Log("[Core.Application] Terminated\n");
}

void Application::Terminate()
{
    Core::Terminate();
}

}
