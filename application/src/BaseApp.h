#pragma once

#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/Clock.h"
#include "Anggur/Graphics/Renderer.h"

using namespace Anggur;

class BaseApp 
{
public:
    BaseApp()
    {
        Initialize();
    }

    virtual void Initialize()
    {
    }

    virtual void Update(float deltaTime)
    {
    }

    void Run()
    {
        Initialize();
        
        float previousTime = clock.GetElapsed();

        while (window.IsOpen())
        {
            float currentTime = clock.GetElapsed();

            windowSystem.PollEvents();

            Update(currentTime - previousTime);

            previousTime = currentTime;

            window.Update();
        }
    }

protected:
    WindowSystem windowSystem;
    Window window;

    Renderer renderer;

    Clock clock;
};