#pragma once

#include <Anggur/Event/Event.h>
#include "Key.h"
#include "MouseButton.h"
#include "Window.h"

namespace Anggur {
    struct WindowEvent: public Event {
        WindowEvent(const std::string& name): Event(name) {
        }
    };

    struct WindowResizeEvent: public WindowEvent {
        Vector2 size;
    };

    struct WindowMoveEvent: public WindowEvent {
        Vector2 position;
    };

    struct MousePressEvent: public WindowEvent {
        MouseButton button; 

        MousePressEvent(MouseButton newButton): WindowEvent("mousePress"), button(newButton) {
        }
    };

    struct MouseReleaseEvent: public WindowEvent {
        MouseButton button; 
        
        MouseReleaseEvent(MouseButton newButton): WindowEvent("mouseRelease"), button(newButton) {
        }
    };

    struct MouseMoveEvent: public WindowEvent {
        MouseMoveEvent(const Vector2& newPosition): WindowEvent("mouseMove"), position(newPosition) {
        }

        Vector2 position; 
    };

    struct MouseScrollEvent: public WindowEvent {
        Vector2 direction;
    };

    struct KeyDownEvent: public WindowEvent {
        Key key;
    };

    struct KeyUpEvent: public WindowEvent {
        Key key;
    };
}
