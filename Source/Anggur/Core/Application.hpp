#pragma once

#include <vector>
#include <Anggur/Renderer/Renderer.hpp>
#include "Audio.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "Scene.hpp"

union SDL_Event;

namespace Anggur {

class Application
{
public:
    Application();

    static Window* GetWindow();
    static void Run(Scene* mainScene = nullptr);
    static void SetWindowConfig(const WindowConfig& config);
    static void SetVsync(bool enable = true);
    static void SetScene(Scene* activeScene);

private:
    static void ProcessEvent(SDL_Event* event);

    static WindowConfig windowConfig;
    static Window* window;
    static Scene* scene;
};

}
