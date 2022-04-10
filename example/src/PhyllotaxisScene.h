#pragma once

#include "Anggur/Core/Application.h"
#include "Anggur/Graphic/Renderer.h"

using namespace Anggur;

class PhyllotaxisScene: public Scene {
    int count = 0;
    float distance = 5.0f;

    void initialize() {
        Vector2 windowSize = Application::getWindow()->getSize();
        Camera camera;
        camera.viewport = windowSize;
        camera.distance = 1.0f / (windowSize.x * 0.5);

        Renderer::setViewProjection(camera.toMatrix3());
    }

    void render() {
        Renderer::clear(Vector4::charcoal);

        for (int i = 0; i < count; ++i) {
            float length = distance * Math::sqrt(i);
            float angle = i * 137.5f;

            Vector2 position = Vector2::createPolar(length, angle);
            Vector4 color = Vector4::lerp(Vector4::orange, Vector4::yellow, Math::normalize(i, 0, count));

            Renderer::drawCircle(transform.toMatrix3(), position, 1, color);
        }

        count += 5;
    }
};
