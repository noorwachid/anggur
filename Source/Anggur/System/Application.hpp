#pragma once

#include <vector>
#include <Anggur/Renderer/Renderer.hpp>
#include "Window.hpp"
#include "Input.hpp"

namespace Anggur {

class Application
{
protected:
    
    virtual void OnInitialize();
    virtual void OnAttach();
    virtual void OnUpdate(float deltaTime);
    virtual void OnEvent(Event* event);
    virtual void OnDetach();

public:

    static Application& Get();

    void Initialize();
    void Run();
    void Terminate();

    Application();

    Window& GetWindow();

private:
    void ProcessEvent(SDL_Event& event);

protected:
    Window* mWindow;
    WindowConfig mWindowConfig;

private:
    static Application* mInstance;
};

}

