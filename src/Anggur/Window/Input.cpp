#pragma once

#include "Input.h"
#include <array>

namespace Anggur 
{
    constexpr int maxKey = 284;
    std::array<bool, maxKey> currentState;
    std::array<bool, maxKey> previousState; 

    bool Input::IsKeyPressed(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index] && previousState[index] == false;
    }

    bool Input::IsKeyReleased(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index] == false && previousState[index];
    }

    bool Input::IsKeyDown(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index];
    }

    bool Input::IsKeyUp(Key key)
    {
        int index = static_cast<int>(key);
        return currentState[index] == false;   
    }

    void Input::Update()
    {
        UpdateKeyState();
    }

    void Input::SetKeyState(Key key, bool state)
    {
        int index = static_cast<int>(key);
        currentState[index] = state;
    }

    void Input::UpdateKeyState()
    {
        for (int i = 0; i < maxKey; ++i) 
        {
            previousState[i] = currentState[i];
        }
    }
}