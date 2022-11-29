#pragma once

#include "Anggur/Event.h"
#include "Anggur/Math/Vector2.h"

namespace Anggur
{
    struct WindowSizeEvent : public Event
    {
        Vector2 size;

        WindowSizeEvent(const std::string &name, const Vector2 &newSize) : Event(name), size(newSize)
        {
        }
    };

    struct WindowPositionEvent : public Event
    {
        Vector2 position;

        WindowPositionEvent(const std::string &name, const Vector2 &newPosition) : Event(name), position(newPosition)
        {
        }
    };

    struct FrameBufferSizeEvent : public Event
    {
        Vector2 size;

        FrameBufferSizeEvent(const std::string &name, const Vector2 &newSize) : Event(name), size(newSize)
        {
        }
    };
}