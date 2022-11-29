#pragma once

#include "Anggur/Graphics/Renderer.h"
#include "Anggur/IO/Clock.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/WindowSystem.h"

using namespace Anggur;

class BaseApp
{
  public:
    BaseApp()
    {
        Initialize();

        // Initialize common inputs
        window.GetInputDevice<Keyboard>();
        window.GetInputDevice<Mouse>();
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