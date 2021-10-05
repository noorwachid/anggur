#pragma once

#include <string>
#include <Anggur/Helper/Enum.h>

struct SDL_Window;

using std::string;

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

ANGGUR_OVERLOAD_ENUM(WindowFlag);

struct WindowConfig
{
    WindowFlag flag;
    uint width;
    uint height;
    string title;
    string icon;

    WindowConfig(WindowFlag flag = WindowFlag::None):
        flag(flag),
        width(480),
        height(360) {}

    WindowConfig(int width, int height, WindowFlag flag = WindowFlag::None):
        flag(flag),
        width(width),
        height(height) {}

    WindowConfig(int width, int height, const string& title, WindowFlag flag = WindowFlag::None):
        flag(flag),
        width(width),
        height(height),
        title(title) {}

    WindowConfig(int width, int height, const string& title, const string& icon, WindowFlag flag = WindowFlag::None):
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

    void SetPosition(const Vector& pos);
    void SetSize(const Vector& size);
    void SetTitle(const string& title);

    float GetRatio();
    Vector GetPosition();
    Vector GetSize();
    const string& GetTitle();
    WindowFlag GetFlag();

    bool IsOpen();
    void Close();
    void SwapBuffers();

    SDL_Window* GetHandler();
    void* GetContext();

    friend class Core;

private:
    SDL_Window* handler;
    void* context;
    float ratio;
    bool isOpen;
    string title;
};

}
