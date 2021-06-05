#pragma once
#include <vector>
#include <Anggur/Math/Vector.hpp>
#include <Anggur/Math/Matrix.hpp>
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "Font.hpp"

namespace Anggur {

enum class DrawMode
{
    Point = 0x1B00,
    Line  = 0x1B01,
    Fill  = 0x1B02,
};

struct Camera
{
public:
    Camera()
    {
        viewport = Vector::one;
        distance = 1.0;
        rotation = 0.0;
    }

    Matrix ToMatrix() const
    {
        float distanceY = distance * (viewport.x / viewport.y);

        return Matrix({
            distance, 0, 0,
            0, distanceY, 0,
            (-origin.x + offset.x) * distance, (-origin.y + offset.y) * distanceY, 1,
        });
    }

    Vector ToWorldCoord(const Vector& screenCoord)
    {
        float distanceY = distance * (viewport.x / viewport.y);
        Vector result(
            (2.0 * screenCoord.x) / viewport.x - 1.0,
           -(2.0 * screenCoord.y) / viewport.y + 1.0
        );

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

    Vector GetVisibleTopLeftWorldCoord()
    {
        return ToWorldCoord(Vector::zero);
    }

    Vector GetVisibleBottomRightWorldCoord()
    {
        return ToWorldCoord(viewport);
    }

    float distance;
    float rotation;

    Vector origin;
    Vector offset;
    Vector viewport;

};

struct Transform
{
    Vector translation;
    float rotation;
    Vector scale;

    Transform(Vector t = Vector(), float r = 0, Vector s = Vector::one):
        translation(t),
        rotation(r),
        scale(s)
    {}    

    Matrix ToMatrix() const
    {
        Matrix mt = Matrix::CreateScale(scale);
        mt.Rotate(rotation);
        mt.Translate(translation);
        return mt;
    }
};

struct TextOption
{
    float size;
    bool xCentering;
    bool yCentering;
};

class Renderer
{
public:
    static Shader& GetBatchShader();

    static void CreateBatchShader();
    static void Initialize();
    static void Terminate();

    static void SetViewport(Vector size);
    static void SetMaxQuad(size_t max = 2048);
    static void SetCircleSegment(size_t segment);

    static void CheckLimit(size_t vertexOffset = 0, size_t indexOffset = 0, size_t textureOffset = 0);
    static void Render();

    static void SetCamera(const Camera& camera);

    // NEW API
    static void AddData(const float* vertexData, size_t vertexLength, const uint* indexData, size_t indexLength);
    static void AddDatax(const Texture& texture);
    static void FlushData();

    static void SetDrawMode(DrawMode mode);

    static void ClearBackground(const Color& color = Color::black);

    // -- Primitives
    static void AddTriangle(const Vector& p0, const Vector& p1, const Vector& p2, const Color& c = Color::white);
    static void AddQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Color& c = Color::white);
    static void AddQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Color& c0, const Color& c1, const Color& c2, const Color& c3);
    static void AddRect(const Vector& p0, float w, float h, const Color& c = Color::white);
    static void AddBox(const Vector& position, const Vector& radii, const Color& c = Color::white);
    static void AddPolygon(const Vector& p0, float r, size_t segments, const Color& c = Color::white);
    static void AddCircle(const Vector& p0, float r, const Color& c = Color::white);

    static void AddTriangle(const Vector& p0, const Vector& p1, const Vector& p2, const Transform& f, const Color& c = Color::white);
    static void AddQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Transform& f, const Color& c = Color::white);
    static void AddRect(const Vector& p0, float w, float h, const Transform& f, const Color& c = Color::white);
    static void AddBox(const Vector& position, const Vector& radii, const Transform& f, const Color& c = Color::white);
    static void AddPolygon(const Vector& p0, float r, size_t segments, const Transform& f, const Color& c = Color::white);
    static void AddCircle(const Vector& p0, float r, const Transform& f, const Color& c = Color::white);

    // -- Textures
    static void Addx(const Vector& p0, const Texture& t, const Color& c = Color::white);
    static void AddQuadx(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3,
                         const Vector& t0, const Vector& t1, const Vector& t2, const Vector& t3,
                         const Texture& t, const Color& c = Color::white);
    static void AddRectx(const Vector& p0, float w, float h, const Texture& t, const Color& c = Color::white);
    static void AddBoxx(const Vector& position, const Vector& radii, const Texture& t, const Color& c = Color::white);

    static void Addx(const Vector& p0, const Texture& t, const Transform& f, const Color& c = Color::white);
    static void AddQuadx(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3,
                         const Vector& t0, const Vector& t1, const Vector& t2, const Vector& t3,
                         const Texture& t, const Transform& f, const Color& c = Color::white);
    static void AddRectx(const Vector& p0, float w, float h, const Texture& t, const Transform& f, const Color& c = Color::white);
    static void AddBoxx(const Vector& position, const Vector& radii, const Texture& t, const Transform& f, const Color& c = Color::white);

    // -- Weirds
    // TODO DrawArc
    static void AddConvex(const std::vector<Vector>& ps, const Color& c = Color::white);

    // -- Lines
    static void AddTerminator(const Vector& p0, const Vector& p1, float w = 0.5, const Color& c = Color::white);
    static void AddAnchor(const Vector& p0, const Vector& p1, const Vector& p2, float w = 0.5, const Color& c = Color::white);
    static void AddLine(const Vector& p0, const Vector& p1, float w = 0.5, const Color& c = Color::white);
    static void AddPolyline(const std::vector<Vector>& ps, float w = 0.5, const Color& c = Color::white);
    static void AddPolyring(const std::vector<Vector>& ps, float w = 0.5, const Color& c = Color::white);

    // -- Curves
    static void AddQuadraticBz(const Vector& p0, const Vector& p1, const Vector& p2, float w = 0.5, const Color& c = Color::white);
    static void AddQuadraticBzi(const Vector& p0, const Vector& p1, const Vector& p2, float w = 0.5, const Color& c = Color::white);
    static void AddQubicBz(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, float w = 0.5, const Color& c = Color::white);

    // -- Texts
    static void AddText(Font& font, int ch, const Vector& p0, float size = 1, const Color& c = Color::white);
    static void AddText(Font& font, const std::string& text, const Vector& p0, float size = 1, const Color& c = Color::white);
    static void AddText(Font& font, const std::string& text, const Vector& p0, const TextOption& o, const Color& c = Color::white);

private:
    static Shader batchShader;

    static float* vertexData;
    static uint* indexData;

    static size_t vertexCounter;
    static size_t indexCounter;

    static size_t maxQuad;
    static size_t circleSegment;

    static size_t maxVertices;
    static size_t maxIndices;

    static size_t maxTextureUnits;
    static size_t textureCounter;
    static float textureIndex;
    static int* textureIndices;
    static Texture* textureData;

    static Matrix viewProjectionMatrix;

    static VertexArray vertexArray;
    static VertexBuffer vertexBuffer;
    static IndexBuffer indexBuffer;

};

}
