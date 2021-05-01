#include <string.h>
#include <Anggur/Helper/Log.hpp>
#include "Input.hpp"

namespace Anggur {

const Uint8* Input::mCurrKeyState = nullptr;
Uint8 Input::mPrevKeyState[SDL_NUM_SCANCODES];

void Input::Initialize()
{
    mCurrKeyState = SDL_GetKeyboardState(nullptr);
    memset(mPrevKeyState, 0, SDL_NUM_SCANCODES);
}

void Input::Update()
{
    memcpy(mPrevKeyState, mCurrKeyState, SDL_NUM_SCANCODES);
}

bool Input::IsKeyPressed(Key key)
{
    int i = SDL_GetScancodeFromKey((int) key);
    return mCurrKeyState[i] == 1 && mPrevKeyState[i] == 0;
}

bool Input::IsKeyHeld(Key key)
{
    int i = SDL_GetScancodeFromKey((int) key);
    return mCurrKeyState[i] && mPrevKeyState[i];
}

bool Input::IsKeyReleased(Key key)
{
    int i = SDL_GetScancodeFromKey((int) key);
    return !mCurrKeyState[i] && mPrevKeyState[i];
}

bool Input::IsScancodePressed(int i)
{
    return mCurrKeyState[i] && !mPrevKeyState[i];
}

bool Input::IsScancodeHeld(int i)
{
    return mCurrKeyState[i] && mPrevKeyState[i];
}

bool Input::IsScancodeReleased(int i)
{
    return !mCurrKeyState[i] && mPrevKeyState[i];
}


}

