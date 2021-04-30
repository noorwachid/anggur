#include <Anggur/Helper/Log.hpp>
#include <Anggur/Renderer/Renderer.hpp>
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

void Application::OnConfigure(WindowConfig& config) {}
void Application::OnAttach() {}
void Application::OnUpdate(float deltaTime) {}
void Application::OnDetach() {}
void Application::OnEvent(Event& event) {}

Application& Application::Get()
{
    return *mInstance;
}

void Application::Initialize()
{
    WindowConfig config;
    OnConfigure(config);
    mWindow = Window::Create(config);

    Anggur_Log("[Core.Application] Initialized\n");
}

void Application::Run()
{
    Initialize();
    Renderer::Initialize();
    OnAttach();

    Uint64 prevTimePoint = 0;

    while (mWindow->IsOpen())
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ProcessEvent(event);
	    // TODO: wrapper event
            //OnEvent(event);
        }

        Uint64 currTimePoint = SDL_GetPerformanceCounter();
        float deltaTime = (currTimePoint - prevTimePoint) / static_cast<float>(SDL_GetPerformanceFrequency());
        prevTimePoint = currTimePoint;

        OnUpdate(deltaTime);

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
