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
    Matrix GetViewProjection() const
    {
        return mView * mProjection;
    }

    float GetApproxScale() const
    {
        return (mProjection[0] + mProjection[4]) * 0.5f;
    }

    void SetRatio(const Vector& size)
    {
        mProjection[4] *= size.x / size.y;
    }

    void SetRatio(float ratio)
    {
        mProjection[4] *= ratio;
    }

    void SetOffset(const Vector& v)
    {
        mProjection.Translate(v);
    }

    void Zoom(const Vector& v)
    {
        mView.Scale(v);
    }

    void Move(const Vector& v)
    {
        mView.Translate(-v);
    }

    void Tilt(const float angle)
    {
        mView.Rotate(-angle);
    }

private:
    Matrix mView;
    Matrix mProjection;

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

class Renderer
{
public:
    static Shader& GetBatchShader();

    static void CreateBatchShader();
    static void Initialize();
    static void Terminate();

    static void SetViewport(uint width, uint height);
    static void SetMaxQuad(size_t max = 2048);
    static void SetCircleSegment(size_t segment);

    static void CheckLimit(size_t vertexOffset = 0, size_t indexOffset = 0, size_t textureOffset = 0);
    static void Render();

    static void BeginScene(const Camera& camera);
    static void EndScene();

    // NEW API
    static void AddData(const float* vertexData, size_t vertexLength, const uint* indexData, size_t indexLength);
    static void AddDatax(const Texture& texture);
    static void FlushData();

    static void SetDrawMode(DrawMode mode);

    static void ClearBackground(const Color& color = Color::charcoal);

    // -- Primitives
    static void AddTriangle(const Vector& p0, const Vector& p1, const Vector& p2, const Color& c = Color::white);
    static void AddQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Color& c = Color::white);
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
    static void AddText(Font& font, const std::string& text, const Vector& position, int size = 50, int chararacterSpacing = 2, int wordSpacing = 5, const Color& c = Color::white);

private:
    static Shader mBatchShader;

    static float* mVertexData;
    static uint* mIndexData;

    static size_t mVertexCounter;
    static size_t mIndexCounter;

    static size_t mMaxQuad;
    static size_t mCircleSegment;

    static size_t mMaxVertices;
    static size_t mMaxIndices;

    static size_t mMaxTextureUnits;
    static size_t mTextureCounter;
    static float mTextureIndex;
    static int* mTextureIndices;
    static Texture* mTextureData;

    static Matrix mViewProjection;

    static VertexArray mVertexArray;
    static VertexBuffer mVertexBuffer;
    static IndexBuffer mIndexBuffer;

};

}
