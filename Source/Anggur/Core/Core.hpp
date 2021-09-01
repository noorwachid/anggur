#pragma once

#include "Window.hpp"
#include "Input.hpp"
#include "Scene.hpp"

union SDL_Event;

namespace Anggur {

class Core
{
public:
    Core();

    static Window* GetWindow();
    static void Run(Scene* mainScene = nullptr);

    static void SetWindowConfig(const WindowConfig& config);
    static void SetVsync(bool enable = true);
    static void SetScene(Scene* activeScene);

    static void Initialize();
    static void Terminate();
    static void LoadGraphicsFunctions();

private:
    static void ProcessEvent(SDL_Event* event);

    static WindowConfig windowConfig;
    static Window* window;
    static Scene* scene;
    static float deltaTime;
    static float elapsedTime;
};

}
