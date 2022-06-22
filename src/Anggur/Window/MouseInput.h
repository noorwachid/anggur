#pragma once

#include <Anggur/Event/EventManager.h>
#include <Anggur/Math/Vector2.h>
#include "Input.h"
#include "WindowEvent.h"
#include <array>

namespace Anggur {
    class MouseInput: public Input {
        MouseInput() {
            EventManager::listen("mouseMove", [&](const Event& event) {
                auto mouseEvent = static_cast<const MouseMoveEvent&>(event);
                position = mouseEvent.position;
            });

            EventManager::listen("mousePress", [&](const Event& event) {
                auto mouseEvent = static_cast<const MousePressEvent&>(event);
                auto index = static_cast<int>(mouseEvent.button);
                previousState[index] = currentState[index];
                currentState[index] = true;
            });

            EventManager::listen("mouseRelease", [&](const Event& event) {
                auto mouseEvent = static_cast<const MouseReleaseEvent&>(event);
                auto index = static_cast<int>(mouseEvent.button);
                previousState[index] = currentState[index];
                currentState[index] = false;
            });
        }

    public:
        const Vector2& getPosition() {
            return position;
        }

        bool isButtonPressed(MouseButton button) {
            auto index = static_cast<int>(button);
            return previousState[index] == false && currentState[index];
        }

        bool isButtonReleased(MouseButton button) {
            auto index = static_cast<int>(button);
            return previousState[index] && currentState[index] == false;
        }

        bool isButtonDown(MouseButton button) {
            auto index = static_cast<int>(button);
            return currentState[index];
        }

        bool isButtonUp(MouseButton button) {
            auto index = static_cast<int>(button);
            return currentState[index] == false;
        }

    private:
        Vector2 position;

        std::array<bool, 0x10> previousState;
        std::array<bool, 0x10> currentState;

        friend class InputManager;
    };
}