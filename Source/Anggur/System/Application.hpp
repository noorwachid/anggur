#pragma once

#include <vector>
#include <Anggur/Renderer/Renderer.hpp>
#include "Window.hpp"
#include "Input.hpp"
#include "Timer.hpp"

union SDL_Event;

namespace Anggur {

class Application
{
protected:
    
    virtual void OnInitialize();
    virtual void OnAttach();
    virtual void OnUpdate(float deltaTime);
    virtual void OnEvent(Event& event);
    virtual void OnDetach();

public:

    static Application& Get();

    void Initialize();
    void Run();
    void Terminate();

    Application();

    Window& GetWindow();

private:
    void ProcessEvent(SDL_Event* event);

protected:
    Window* mWindow;
    WindowConfig mWindowConfig;

private:
    static Application* mInstance;
};

}

#ifndef Anggur_EntryPoint
#define Anggur_EntryPoint(applicationInstanceName) \
    int main() { \
        applicationInstanceName applicationInstance; \
        applicationInstance.Run(); \
        return 0; \
    }
#endif
