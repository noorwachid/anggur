#pragma once
#include "Color.h"
#include "Font.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "RendererStruct.h"
#include <Anggur/Math/Matrix.h>
#include <Anggur/Math/Vector.h>
#include <vector>

namespace Anggur
{

class Renderer
{
public:
    static Shader& GetBatchShader();

    static void CreateBatchShader();
    static void Initialize();
    static void Terminate();

    static void SetDrawMode(DrawMode mode);
    static void SetMaxQuad(size_t max = 2048);
    static void SetCircleSegment(size_t segment);


    static void SetViewport(Vector size);
    static void SetViewProjectionMatrix(const Matrix& matrix);

    static void ClearBackground(const Color& color = Color::black);
    static void CheckLimit(size_t vertexOffset = 0, size_t indexOffset = 0, size_t textureOffset = 0);
    static void AddData(const float* vertexData, size_t vertexLength, const uint* indexData, size_t indexLength);
    static void AddTextureData(const Texture& texture);
    static void ClearData();
    static void Render();

    // NEW API

    // -- Primitives
    static void AddTriangle(const Transform& transform, const Vector& p0 = Vector(0.0, -0.5), const Vector& p1 = Vector(0.5, 0.5), const Vector& p2 = Vector(-0.5, 0.5), const Color& c = Color::white);
    static void AddQuad(const Transform& transform, const Vector& p0 = Vector(-0.5, -0.5), const Vector& p1 = Vector(0.5, -0.5), const Vector& p2 = Vector(0.5, 0.5), const Vector& p3 = Vector(-0.5, 0.5), const Color& c = Color::white);
    static void AddRect(const Transform& transform, const Vector& p0 = Vector(-0.5, -0.5), const Vector& p1 = Vector(0.5, 0.5), const Color& color = Color::white);
    static void AddPolygon(const Transform& transform, const Vector& p0, float r = 0.5, size_t segments = 6, const Color& c = Color::white);
    static void AddCircle(const Transform& transform, const Vector& p0, float r = 0.5, const Color& c = Color::white);

    // -- Lines
    static void AddTerminator(const Transform& transform, const Vector& p0, const Vector& p1, float w = 0.5, const Color& c = Color::white);
    static void AddAnchor(const Transform& transform, const Vector& p0, const Vector& p1, const Vector& p2, float w = 0.5, const Color& c = Color::white);
    static void AddLine(const Transform& transform, const Vector& p0, const Vector& p1, float w = 0.5, const Color& c = Color::white);
    static void AddPolyline(const Transform& transform, const std::vector<Vector>& ps, float w = 0.5, const Color& c = Color::white);
    static void AddPolyring(const Transform& transform, const std::vector<Vector>& ps, float w = 0.5, const Color& c = Color::white);

    // -- Curves
    static void AddQuadraticBz(const Transform& transform, const Vector& p0, const Vector& p1, const Vector& p2, float w = 0.5, const Color& c = Color::white);
    static void AddQuadraticBzi(const Transform& transform, const Vector& p0, const Vector& p1, const Vector& p2, float w = 0.5, const Color& c = Color::white);
    static void AddQubicBz(const Transform& transform, const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, float w = 0.5, const Color& c = Color::white);

    // -- Natural
    // TODO DrawArc
    static void AddConvex(const Transform& transform, const std::vector<Vector>& ps, const Color& c = Color::white);


    // -- Text
    static void AddText(const Transform& transform, const Vector& p0, const Vector& p1, const string& textBuffer, const TextOption& textOption, Font& textFont, const Color& color = Color::white);
    static void AddTextChunk(const Transform &transform, const Vector& p0, const std::vector<CodepointContainer>& ccs, Font &textFont, const Color &color);

    // OLD API
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

} // namespace Anggur
