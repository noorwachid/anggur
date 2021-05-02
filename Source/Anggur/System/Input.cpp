#include <string.h>
#include <Anggur/Helper/Log.hpp>
#include "Input.hpp"

namespace Anggur {

SDL_Window* Input::mRawWindow = nullptr;

const Uint8* Input::mKeyCurrState = nullptr;
Uint8 Input::mKeyPrevState[SDL_NUM_SCANCODES];

Uint32 Input::mMouseCurrState = 0;
Uint32 Input::mMousePrevState = 0;
Vector Input::mMousePos;
Vector Input::mMouseWheel;

void Input::Initialize()
{
    mKeyCurrState = SDL_GetKeyboardState(nullptr);
    memset(mKeyPrevState, 0, SDL_NUM_SCANCODES);
}

void Input::PreUpdate()
{
    memcpy(mKeyPrevState, mKeyCurrState, SDL_NUM_SCANCODES);

    mMousePrevState = mMouseCurrState;
    mMouseWheel.Set(0, 0);
}

void Input::Update()
{
    int x, y;
    mMouseCurrState = SDL_GetMouseState(&x, &y);
    mMousePos = Vector(x, y);
}

bool Input::IsKeyPressed(Key key)
{
    int i = SDL_GetScancodeFromKey((int) key);
    return mKeyCurrState[i] == 1 && mKeyPrevState[i] == 0;
}

bool Input::IsKeyHeld(Key key)
{
    int i = SDL_GetScancodeFromKey((int) key);
    return mKeyCurrState[i] && mKeyPrevState[i];
}

bool Input::IsKeyReleased(Key key)
{
    int i = SDL_GetScancodeFromKey((int) key);
    return !mKeyCurrState[i] && mKeyPrevState[i];
}

bool Input::IsScancodePressed(int i)
{
    return mKeyCurrState[i] && !mKeyPrevState[i];
}

bool Input::IsScancodeHeld(int i)
{
    return mKeyCurrState[i] && mKeyPrevState[i];
}

bool Input::IsScancodeReleased(int i)
{
    return !mKeyCurrState[i] && mKeyPrevState[i];
}

bool Input::IsMousePressed(Mouse button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return !(mMousePrevState & btn) && (mMouseCurrState & btn);
}

bool Input::IsMouseHeld(Mouse button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return (mMousePrevState & btn) && (mMouseCurrState & btn);
}

bool Input::IsMouseReleased(Mouse button)
{
    Uint32 btn = static_cast<Uint32>(button);
    return (mMousePrevState & btn) && !(mMouseCurrState & btn);
}

bool Input::IsMouseScrolled()
{
    return mMouseWheel.y != 0.0f || mMouseWheel.x != 0.0f;
}

void Input::SetMousePos(const Vector& pos)
{
    int x = pos.x;
    int y = pos.y;
    SDL_WarpMouseInWindow(mRawWindow, x, y);
}

Vector Input::GetMousePos()
{
    return mMousePos;
}

Vector Input::GetMouseWheel()
{
    return mMouseWheel;
}


}

