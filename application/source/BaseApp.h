#pragma once

#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/Clock.h"
#include "Anggur/Graphics/Renderer.h"

using namespace Anggur;

class BaseApp {
public:
    BaseApp() {
        initialize();

        // Initialize common inputs
        window.getInputDevice<Keyboard>();
        window.getInputDevice<Mouse>();
    }

    virtual void initialize() {
    }

    virtual void update(float deltaTime) {
    }

    void run() {
        initialize();
        
        float previousTime = clock.getElapsed();

        while (window.isOpen()) {
            float currentTime = clock.getElapsed();

            windowSystem.pollEvents();

            update(currentTime - previousTime);

            previousTime = currentTime;

            window.update();
        }
    }

protected:
    WindowSystem windowSystem;
    Window window;

    Renderer renderer;

    Clock clock;
};