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
        while (SDL_PollEvent(&event))
        {
            ProcessEvent(event);
        }

        Uint64 currTimePoint = SDL_GetPerformanceCounter();
        float dx = (currTimePoint - prevTimePoint) / static_cast<float>(SDL_GetPerformanceFrequency());
        prevTimePoint = currTimePoint;

        OnUpdate(dx);
        Input::Update();

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
