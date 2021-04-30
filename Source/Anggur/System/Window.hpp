#pragma once

#include <string>
#include <Anggur/Helper/Enum.hpp>
#include "Core.hpp"

namespace Anggur {

class Event;
class Vector;
class Image;

struct WindowConfig
{
    uint width;
    uint height;
    std::string title;
    std::string icon;

    WindowConfig():
        width(480), height(360),
        title("Untitled")
    {}
};

struct WindowSize
{
    uint width;
    uint height;

    WindowSize(uint w, uint h):
        width(w), height(h)
    {}
};

class Window
{
    // TODO: Implement subwindow

public:
    static Window* Create(const WindowConfig& config = WindowConfig()); // only can make one 'main' window

    float GetRatio();
    WindowSize GetSize();

    bool IsOpen();
    void Close();
    void SwapBuffers();

    SDL_Window* GetRawWindow();
    SDL_GLContext GetContext();

private:
    SDL_Window* mRawWindow;
    SDL_GLContext mContext;

    float mRatio;
    bool mOpen;

    Window(const WindowConfig& config = WindowConfig());
};

}
