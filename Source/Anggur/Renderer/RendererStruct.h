#pragma once

#include <Anggur/Math/Matrix.h>
#include <Anggur/Math/Vector.h>

namespace Anggur {


enum class DrawMode
{
    Point = 0x1B00,
    Line = 0x1B01,
    Fill = 0x1B02,
};

struct Camera
{
public:
    Camera()
    {
        origin = Vector::zero;
        offset = Vector::zero;
        viewport = Vector::one;
        distance = 1.0;
        rotation = 0.0;
    }

    Matrix ToMatrix() const
    {
        float distanceY = -distance * (viewport.x / viewport.y);
        Matrix matrix({
            distance, 0, 0,
            0, distanceY, 0,
            (-origin.x + offset.x) * distance, (-origin.y + offset.y) * distanceY, 1,
        });
        //        matrix.Rotate(rotation);
        return matrix;
    }

    Vector ScreenToWorldPoint(const Vector& screenPoint)
    {
        Vector result(
            (2.0 * screenPoint.x) / viewport.x - 1.0,
            -(2.0 * screenPoint.y) / viewport.y + 1.0);

        return result * Matrix::CreateInverse(ToMatrix());
    }

    Vector WorldToScreenPoint(const Vector& screenPoint)
    {
        Vector result(
            -screenPoint.x / viewport.x,
            screenPoint.y / viewport.y);
        return result * Matrix::CreateInverse(ToMatrix());
    }

    void SetRatio(float ratio)
    {
        viewport.y = viewport.x * ratio;
    }

    float GetRatio()
    {
        return viewport.x / viewport.y;
    }

    float distance;
    float rotation;

    Vector origin;
    Vector offset;
    Vector viewport;
};

struct TextOption
{
    float size;
    float lineHeight;
    float letterSpace;
    float wordSpace;
    float isBreakable;
    string ellipsis;

    TextOption(float size = 1, float lineHeight = 1.3, float wordSpace = 0.2, float letterSpace = 0.05, bool isBreakable = false, const string& ellipsis = "..."):
        size(size),
        lineHeight(lineHeight),
        wordSpace(wordSpace),
        letterSpace(letterSpace),
        isBreakable(isBreakable),
        ellipsis(ellipsis)
    {}
};

struct CodepointContainer
{
    Glyph glyph;
    Vector offset;
    Vector area;
};

}
