#pragma once

#include "Key.h"
#include "MouseButton.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Event.h"

namespace Anggur 
{
    struct KeyEvent: public Event 
    {
        Key key = Key::Unknown;

        KeyEvent(const std::string& name, Key newKey): Event(name), key(newKey) 
        {
        }
    };

    struct MouseButtonEvent: public Event 
    {
        MouseButton button = MouseButton::Unknown;

        MouseButtonEvent(const std::string& name, MouseButton newButton): Event(name), button(newButton) 
        {
        }
    };

    struct MousePositionEvent: public Event 
    {
        Vector2 position;

        MousePositionEvent(const std::string& name, const Vector2& newPosition): Event(name), position(newPosition) 
        {
        }
    };

    struct ScrollEvent: public Event 
    {
        Vector2 direction;

        ScrollEvent(const std::string& name, const Vector2& newDirection): Event(name), direction(newDirection) 
        {
        }
    };

    struct CodePointEvent: public Event
    {
        uint32_t codePoint;

        CodePointEvent(uint32_t newCodePoint): Event("CodePoint"), codePoint(newCodePoint)
        {
        }
    };
}
