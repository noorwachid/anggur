#include <string.h>
#include <SDL_events.h>
#include <Anggur/Helper/Log.h>
#include "Input.h"

namespace Anggur {

SDL_Window* Input::windowHandler = nullptr;

const uint8_t* Input::currScancodeMap = nullptr;
uint8_t Input::prevScancodeMap[Anggur_ScancodeLength];

uint Input::currButtonMap = 0;
uint Input::prevButtonMap = 0;
Vector Input::cursor;
Vector Input::wheel;

void Input::Initialize()
{
    currScancodeMap = SDL_GetKeyboardState(nullptr);
    memset(prevScancodeMap, 0, Anggur_ScancodeLength);
}

void Input::PreUpdate()
{
    memcpy(prevScancodeMap, currScancodeMap, Anggur_ScancodeLength);

    prevButtonMap = currButtonMap;
    wheel.Set(0, 0);
}

void Input::Update()
{
    int x, y;
    currButtonMap = SDL_GetMouseState(&x, &y);
    cursor = Vector(x, y);
}

bool Input::IsKeyPressed(Scancode scancode)
{
    int i = static_cast<int>(scancode);
    return currScancodeMap[i] == 1 && prevScancodeMap[i] == 0;
}

bool Input::IsKeyHeld(Scancode scancode)
{
    int i = static_cast<int>(scancode);
    return currScancodeMap[i] && prevScancodeMap[i];
}

bool Input::IsKeyReleased(Scancode scancode)
{
    int i = static_cast<int>(scancode);
    return !currScancodeMap[i] && prevScancodeMap[i];
}

bool Input::IsKeyPressed(Key key)
{
    int i = SDL_GetScancodeFromKey(static_cast<int>(key));
    return currScancodeMap[i] == 1 && prevScancodeMap[i] == 0;
}

bool Input::IsKeyHeld(Key key)
{
    int i = SDL_GetScancodeFromKey(static_cast<int>(key));
    return currScancodeMap[i] && prevScancodeMap[i];
}

bool Input::IsKeyReleased(Key key)
{
    int i = SDL_GetScancodeFromKey(static_cast<int>(key));
    return !currScancodeMap[i] && prevScancodeMap[i];
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return !(prevButtonMap & btn) && (currButtonMap & btn);
}

bool Input::IsMouseButtonHeld(MouseButton button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return (prevButtonMap & btn) && (currButtonMap & btn);
}

bool Input::IsMouseButtonReleased(MouseButton button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return (prevButtonMap & btn) && !(currButtonMap & btn);
}

void Input::SetMouseCursor(const Vector& pos)
{
    int x = pos.x;
    int y = pos.y;
    SDL_WarpMouseInWindow(windowHandler, x, y);
}

Vector Input::GetMouseCursor()
{
    return cursor;
}

Vector Input::GetMouseWheel()
{
    return wheel;
}


}

