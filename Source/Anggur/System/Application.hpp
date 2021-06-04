#pragma once

#include <vector>
#include <Anggur/Renderer/Renderer.hpp>
#include "Audio.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Timer.hpp"

union SDL_Event;

namespace Anggur {

class Application
{
protected:
    
    virtual void OnCreate();
    virtual void OnStart();
    virtual void OnUpdate();
    virtual void OnDestroy();

    virtual void OnEvent(Event& event);

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
    WindowConfig windowConfig;
    Window* window;

private:
    static Application* instance;
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
