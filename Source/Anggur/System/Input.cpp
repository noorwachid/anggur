#include <string.h>
#include <SDL_events.h>
#include <Anggur/Helper/Log.hpp>
#include "Input.hpp"

namespace Anggur {

SDL_Window* Input::windowHandler = nullptr;

const uchar* Input::keyCurrState = nullptr;
uchar Input::keyPrevState[Anggur_ScancodeLength];

uint Input::mouseCurrState = 0;
uint Input::mousePrevState = 0;
Vector Input::mousePos;
Vector Input::mouseWheel;

void Input::Initialize()
{
    keyCurrState = SDL_GetKeyboardState(nullptr);
    memset(keyPrevState, 0, Anggur_ScancodeLength);
}

void Input::PreUpdate()
{
    memcpy(keyPrevState, keyCurrState, Anggur_ScancodeLength);

    mousePrevState = mouseCurrState;
    mouseWheel.Set(0, 0);
}

void Input::Update()
{
    int x, y;
    mouseCurrState = SDL_GetMouseState(&x, &y);
    mousePos = Vector(x, y);
}

bool Input::IsPressed(Key key)
{
    int i = static_cast<int>(key);
    return keyCurrState[i] == 1 && keyPrevState[i] == 0;
}

bool Input::IsHeld(Key key)
{
    int i = static_cast<int>(key);
    return keyCurrState[i] && keyPrevState[i];
}

bool Input::IsReleased(Key key)
{
    int i = static_cast<int>(key);
    return !keyCurrState[i] && keyPrevState[i];
}

bool Input::IsPressed(VirtKey vir)
{
    int i = SDL_GetScancodeFromKey(static_cast<int>(vir));
    return keyCurrState[i] == 1 && keyPrevState[i] == 0;
}

bool Input::IsHeld(VirtKey vir)
{
    int i = SDL_GetScancodeFromKey(static_cast<int>(vir));
    return keyCurrState[i] && keyPrevState[i];
}

bool Input::IsReleased(VirtKey vir)
{
    int i = SDL_GetScancodeFromKey(static_cast<int>(vir));
    return !keyCurrState[i] && keyPrevState[i];
}

bool Input::IsPressed(MouseButton button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return !(mousePrevState & btn) && (mouseCurrState & btn);
}

bool Input::IsHeld(MouseButton button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return (mousePrevState & btn) && (mouseCurrState & btn);
}

bool Input::IsReleased(MouseButton button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return (mousePrevState & btn) && !(mouseCurrState & btn);
}

bool Input::IsWheelScrolled()
{
    return mouseWheel.y != 0.0f || mouseWheel.x != 0.0f;
}

void Input::SetCursorPos(const Vector& pos)
{
    int x = pos.x;
    int y = pos.y;
    SDL_WarpMouseInWindow(windowHandler, x, y);
}

Vector Input::GetCursorPos()
{
    return mousePos;
}

Vector Input::GetWheel()
{
    return mouseWheel;
}


}

