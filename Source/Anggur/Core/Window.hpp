#pragma once

#include <string>
#include <Anggur/Helper/Enum.hpp>
#include "Core.hpp"

struct SDL_Window;

namespace Anggur {

class Event;
class Vector;
class Image;

enum struct WindowFlag: int
{
    None = 0x0,

    FullscreenDesktop = (0x00000001|0x00001000),

    Fullscreen 	 = 0x00000001,
    Shown      	 = 0x00000004,
    Hidden     	 = 0x00000008,
    Borderless 	 = 0x00000010,
    Resizable  	 = 0x00000020,
    Minimized  	 = 0x00000040,
    Maximized  	 = 0x00000080,
    InputGrabbed = 0x00000100,
    InputFocus   = 0x00000200,
    MouseFocus   = 0x00000400,
    Foreign      = 0x00000800,
    AllowHighDpi = 0x00002000,

    MouseCapture = 0x00004000,
    AlwaysOnTop  = 0x00008000,
    SkipTaskbar  = 0x00010000,
    Utility      = 0x00020000,
    Tooltip      = 0x00040000,
    PopupMenu    = 0x00080000,
};

Anggur_EnumOverloads(WindowFlag);

struct WindowConfig
{
    WindowFlag flag;
    uint width;
    uint height;
    std::string title;
    std::string icon;

    WindowConfig(WindowFlag flag = WindowFlag::None):
        flag(flag),
        width(480),
        height(360) {}

    WindowConfig(int width, int height, WindowFlag flag = WindowFlag::None):
        flag(flag),
        width(width),
        height(height) {}

    WindowConfig(int width, int height, const std::string& title, WindowFlag flag = WindowFlag::None):
        flag(flag),
        width(width),
        height(height),
        title(title) {}

    WindowConfig(int width, int height, const std::string& title, const std::string& icon, WindowFlag flag = WindowFlag::None):
        flag(flag),
        width(width),
        height(height),
        title(title),
        icon(icon) {}
};

class Window
{
    // TODO: Implement subwindow

public:
    Window(const WindowConfig& config = WindowConfig());

    void SetPos(const Vector& pos);
    void SetSize(const Vector& size);
    void SetTitle(const std::string& title);

    float GetRatio();
    Vector GetPos();
    Vector GetSize();
    const std::string& GetTitle();
    WindowFlag GetFlag();

    bool IsOpen();
    void Close();
    void SwapBuffers();

    SDL_Window* GetRawWindow();
    void* GetContext();

    friend class Application;

private:
    SDL_Window* handler;
    void* context;
    float ratio;
    bool open;
    std::string title;
};

}
