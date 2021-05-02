#include <glad/glad.h>
#include <Anggur/Helper/Log.hpp>
#include "Renderer.hpp"

namespace Anggur {

struct Vertex
{
    float position[2];
    float color[4];
    float texCoord[2];
    float texIndex;

    static const size_t length = 9;
};

Shader Renderer::mBatchShader;

float* Renderer::mVertexData;
uint* Renderer::mIndexData;

size_t Renderer::mVertexCounter;
size_t Renderer::mIndexCounter;

size_t Renderer::mMaxQuad = 2048;

size_t Renderer::mMaxVertices;
size_t Renderer::mMaxIndices;

size_t Renderer::mMaxTextureUnits;
size_t Renderer::mTextureCounter;
float Renderer::mTextureIndex;
int* Renderer::mTextureIndices;
Texture* Renderer::mTextureData;

Matrix Renderer::mViewProjection;

VertexArray Renderer::mVertexArray;
VertexBuffer Renderer::mVertexBuffer;
IndexBuffer Renderer::mIndexBuffer;

Shader& Renderer::GetBatchShader()
{
    return mBatchShader;
}

void Renderer::CreateBatchShader()
{
    mBatchShader.SetVertexSource(R"(
        #version 330 core
        layout (location = 0) in vec2  aPosition;
        layout (location = 1) in vec4  aColor;
        layout (location = 2) in vec2  aTexCoord;
        layout (location = 3) in float aTexIndex;

        out vec4  vColor;
        out vec2  vTexCoord;
        out float vTexIndex;

        uniform mat3 uViewProjection;

        void main()
        {
        gl_Position       = vec4(uViewProjection * vec3(aPosition, 1.0f), 1.0f);
        vColor      	  = aColor;
        vTexCoord   	  = aTexCoord;
        vTexIndex   	  = aTexIndex;
        }
    )");

    std::string fragmentSource = R"(
        #version 330 core

        layout (location = 0) out vec4 aColor;

        in vec4  vColor;
        in vec2  vTexCoord;
        in float vTexIndex;

        uniform sampler2D uTex[16];

        void main()
        {
            int index = int(vTexIndex);

            switch (index)
            {
            // MULTI CHANNELS TEXTURE
            case -1:
                aColor = vColor;
                break;
    )";

    int maxUnit;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnit);
    mMaxTextureUnits = maxUnit;

    mTextureData = new Texture[mMaxTextureUnits];
    mTextureIndices = new int[mMaxTextureUnits];

    for (size_t i = 0; i < mMaxTextureUnits; ++i)
    {
        mTextureIndices[i] = i;

        fragmentSource += "case " + std::to_string(i) + ":\n";
        fragmentSource += "aColor = texture(uTex[" + std::to_string(i) + "], vTexCoord) * vColor;\n";
        fragmentSource += "break;\n";

        fragmentSource += "case " + std::to_string(i + mMaxTextureUnits) + ":\n";
        fragmentSource += "aColor = vec4(1.f, 1.f, 1.f, texture(uTex[" + std::to_string(i) + "], vTexCoord)) * vColor;\n";
        fragmentSource += "break;\n";
    }

    fragmentSource += R"(
            }
        }
    )";

    mBatchShader.SetFragmentSource(fragmentSource);
    mBatchShader.Compile();

}

void Renderer::Initialize()
{
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CreateBatchShader();

    mMaxVertices = mMaxQuad * 4;
    mMaxIndices  = mMaxQuad * 6;

    mVertexArray.Create();
    mVertexArray.Bind();
    mVertexData = new float[mMaxVertices * Vertex::length];
    mIndexData = new uint[mMaxIndices];

    mVertexBuffer.Create();
    mVertexBuffer.Bind();
    mVertexBuffer.SetCapacity(sizeof(float) * mMaxVertices * Vertex::length);

    mVertexArray.SetAttributePtr(0, 2, sizeof(Vertex), (void*) offsetof(Vertex, position));
    mVertexArray.SetAttributePtr(1, 4, sizeof(Vertex), (void*) offsetof(Vertex, color));
    mVertexArray.SetAttributePtr(2, 2, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));
    mVertexArray.SetAttributePtr(3, 1, sizeof(Vertex), (void*) offsetof(Vertex, texIndex));

    mIndexBuffer.Create();
    mIndexBuffer.Bind();
    mIndexBuffer.SetCapacity(sizeof(uint) * mMaxIndices);

    FlushData();
}

void Renderer::Terminate()
{
    delete[] mVertexData;
    delete[] mIndexData;
    delete[] mTextureIndices;
    delete[] mTextureData;

    mVertexData = nullptr;
    mIndexData = nullptr;
    mTextureIndices = nullptr;
    mTextureData = nullptr;

    mIndexBuffer.Destroy();
    mVertexBuffer.Destroy();
    mVertexArray.Destroy();
    mBatchShader.Destroy();
}

void Renderer::SetMaxQuad(size_t max)
{
    if (max < 32) max = 32;
    mMaxQuad = max;
}

void Renderer::ClearBackground(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Render()
{
    if (mVertexCounter == 0)
        return;

    mBatchShader.Bind();
    mBatchShader.SetMatrix("uViewProjection", mViewProjection);
    mBatchShader.SetInt("uTex", mMaxTextureUnits, mTextureIndices);

    for (size_t i = 0; i < mTextureCounter; ++i)
        mTextureData[i].Bind(i);

    mVertexBuffer.Bind();
    mVertexBuffer.SetData(sizeof(float) * mMaxVertices * Vertex::length, mVertexData);

    mIndexBuffer.Bind();
    mIndexBuffer.SetData(sizeof(uint) * mMaxIndices, mIndexData);

    mVertexArray.Bind();
    glDrawElements(GL_TRIANGLES, mIndexCounter, GL_UNSIGNED_INT, nullptr);

    FlushData();
}

void Renderer::BeginScene(const Camera& camera)
{
   mViewProjection = camera.GetViewProjection();
}

void Renderer::EndScene()
{
    Render();
    mViewProjection = Matrix::identity;
}

void Renderer::FlushData()
{
    mViewProjection = Matrix::identity;
    mVertexCounter = 0;
    mIndexCounter = 0;
    mTextureCounter = 0;
}

void Renderer::CheckLimit(size_t vertexOffset, size_t indexOffset, size_t textureOffset)
{
    if (mVertexCounter + vertexOffset >= mMaxVertices)
        return Render();
    if (mIndexCounter + indexOffset >= mMaxIndices)
        return Render();
    if (mTextureCounter + textureOffset >= mMaxTextureUnits)
        return Render();
}

void Renderer::AddText(Font& font, const std::string& text, const Vector& position, int size, int characterSpacing, int wordSpacing, const Color& color)
{
    float height = font.GetTexture().GetHeight();
    float x = 0;

    for (auto c: text)
    {
        if (c == 32) x += wordSpacing;

        CharRect ci = font.GetCharRect(c);

        float scaledWidth = (size / height) * ci.width;

        /*
        drawRectxc(
            {position.x + x, position.y},
            scaledWidth,
            size,
            {ci.x * 1.f, 0},
            ci.width, height,
            font.texture(),
            color
        );
        */

        x += scaledWidth + characterSpacing;
    }
}

void Renderer::AddData(const float* vetices, size_t vertexLength,
                          const uint* indices, size_t indexLength)
{
    size_t vertexOffset = mVertexCounter * Vertex::length;
    size_t vertexSize   = vertexLength * Vertex::length;

    for (size_t i = 0; i < vertexSize; ++i)
        mVertexData[vertexOffset + i] = vetices[i];

    for (size_t i = 0; i < indexLength; ++i)
        mIndexData[mIndexCounter + i] = mVertexCounter + indices[i];

    mVertexCounter += vertexLength;
    mIndexCounter += indexLength;
}

void Renderer::AddDatax(const Texture& texture)
{
    for (size_t i = 0; i < mTextureCounter; ++i)
    {
        if (mTextureData[i].GetId() == texture.GetId())
        {
            mTextureIndex = i;
            return;
        }
    }

    mTextureData[mTextureCounter] = texture;
    mTextureIndex = mTextureCounter;
    mTextureCounter++;
}


void Renderer::SetDrawMode(DrawMode mode)
{
    glPolygonMode(GL_FRONT_AND_BACK, static_cast<int>(mode));
}

void Renderer::AddTriangle(const Vector& p0, const Vector& p1, const Vector& p2, const Color& c)
{
    CheckLimit(3, 3);

    float vertices[] = {
        p0.x, p0.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
        p1.x, p1.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
        p2.x, p2.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
    };

    uint indices[] = { 0, 1, 2 };

    AddData(vertices, 3, indices, 3);
}

void Renderer::AddQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Color& c)
{
    CheckLimit(4, 6);

    float vertices[] = {
        p0.x, p0.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
        p1.x, p1.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
        p2.x, p2.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
        p3.x, p3.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
    };

    uint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    AddData(vertices, 4, indices, 6);
}

void Renderer::AddRect(const Vector& p0, float w, float h, const Color& c)
{
    AddQuad(
        {p0.x,     p0.y},
        {p0.x + w, p0.y},
        {p0.x + w, p0.y + h},
        {p0.x,     p0.y + h},
        c
    );
}

void Renderer::AddBox(const Vector& position, const Vector& radii, const Color& c)
{
    AddQuad(
        {position.x - radii.x, position.y - radii.y},
        {position.x + radii.x, position.y - radii.y},
        {position.x + radii.x, position.y + radii.y},
        {position.x - radii.x, position.y + radii.y},
        c
    );
}

void Renderer::AddConvex(const std::vector<Vector>& ps, const Color& c)
{
    size_t triangles = ps.size() - 2;
    uint indices[triangles * 3];
    float vertices[ps.size() * Vertex::length];

    for (size_t i = 0, offset = 0; i < ps.size(); ++i)
    {
        vertices[offset + 0] = ps[i].x;
        vertices[offset + 1] = ps[i].y;

        vertices[offset + 2] = c.r;
        vertices[offset + 3] = c.g;
        vertices[offset + 4] = c.b;
        vertices[offset + 5] = c.a;

        vertices[offset + 6] = 0;
        vertices[offset + 7] = 0;

        vertices[offset + 8] = -1;

        offset += Vertex::length;
    }

    for (size_t i = 0, offset = 0; i < triangles; ++i)
    {
        indices[offset + 0] = 0;
        indices[offset + 1] = i + 1;
        indices[offset + 2] = i + 2;

        offset += 3;
    }

    AddData(vertices, ps.size(), indices, triangles * 3);
}

void Renderer::AddPolygon(const Vector& p0, float r, size_t segments, const Color& c)
{
    if (segments < 3) segments = 3;

    size_t triangles = segments - 2;

    CheckLimit(segments, triangles * 3);

    float theta           = Math::pi2 / segments;
    float tangetialFactor = Math::Tan(theta);
    float radialFactor    = Math::Cos(theta);

    float x = r;
    float y = 0;

    float vertices[segments * Vertex::length];
    uint indices[triangles * 3];

    for (size_t i = 0, offset = 0; i < segments; i++)
    {
        vertices[offset + 0] = x + p0.x;
        vertices[offset + 1] = y + p0.y;

        vertices[offset + 2] = c.r;
        vertices[offset + 3] = c.g;
        vertices[offset + 4] = c.b;
        vertices[offset + 5] = c.a;

        vertices[offset + 6] = 0;
        vertices[offset + 7] = 0;

        vertices[offset + 8] = -1;

        float tx = -y;
        float ty = x;

        x += tx * tangetialFactor;
        y += ty * tangetialFactor;

        x *= radialFactor;
        y *= radialFactor;

        offset += Vertex::length;
    }

    for (size_t i = 0, offset = 0; i < triangles; ++i)
    {
        indices[offset + 0] = 0;
        indices[offset + 1] = i + 1;
        indices[offset + 2] = i + 2;
        offset += 3;
    }

    AddData(vertices, segments, indices, triangles * 3);
}

void Renderer::AddCircle(const Vector& p0, float r, const Color& c)
{
    AddPolygon(p0, r, 32, c);
}


void Renderer::AddQuadx(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Vector& t0, const Vector& t1, const Vector& t2, const Vector& t3, const Texture& t, const Color& c)
{
    CheckLimit(4, 6, 1);

    AddDatax(t);
    float ti = mTextureIndex;
    if (t.GetChannels() == 1)
        ti += mMaxTextureUnits;

    float vertices[] = {
        p0.x, p0.y, c.r, c.g, c.b, c.a, t0.x, t0.y, ti,
        p1.x, p1.y,	c.r, c.g, c.b, c.a, t1.x, t1.y, ti,
        p2.x, p2.y, c.r, c.g, c.b, c.a, t2.x, t2.y, ti,
        p3.x, p3.y, c.r, c.g, c.b, c.a, t3.x, t3.y, ti,
    };

    uint indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    AddData(vertices, 4, indices, 6);
}

void Renderer::Addx(const Vector& p0, const Texture& t, const Color& c)
{
    float w = t.GetWidth();
    float h = t.GetHeight();

    AddQuadx(
        p0,
        {p0.x + w, p0.y},
        {p0.x + w, p0.y + h},
        {p0.x, p0.y + h},
        {0.f, 0.f},
        {1.f, 0.f},
        {1.f, 1.f},
        {0.f, 1.f},
        t,
        c
    );
}

void Renderer::AddRectx(const Vector& p0, float w, float h, const Texture& t, const Color& c)
{
    AddQuadx(
        p0,
        {p0.x + w, p0.y},
        {p0.x + w, p0.y + h},
        {p0.x, p0.y + h},
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1},
        t,
        c
    );
}

void Renderer::AddTerminator(const Vector& p0, const Vector& p1, float weight, const Color& c)
{
    Vector p3 = (p0 - p1).Normalize() * weight;
    Vector t0 = (p1 - p0).GetPerpendicular().Normalize() * weight;

    AddQuad(p1 + t0, p3 + p0 + t0, p3 + p0 - t0, p1 - t0, c);
}

void Renderer::AddAnchor(const Vector& p0, const Vector& p1, const Vector& p2, float w0, const Color& c)
{
    Vector t0 = (p1 - p0).GetPerpendicular();
    Vector t2 = (p2 - p1).GetPerpendicular();

    if (0 < ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y)))
    {
        t0 = -t0;
        t2 = -t2;
    }

    t0.SetLength(w0);
    t2.SetLength(w0);

    Vector i0;
    Vector u0 = p0 + t0;
    Vector u1 = p2 + t2;
    Vector n0 = p0 - t0;
    Vector n1 = p2 - t2;
    Vector c0 = p1 + t0;
    Vector c1 = p1 + t2;
    Vector d0 = p1 - t0;
    Vector d1 = p1 - t2;
    Vector e0 = (p1 - p0).SetLength(w0 * 2) + c0;
    Vector e1 = (p1 - p2).SetLength(w0 * 2) + c1;

    auto areLinesIntersected = [](
        const Vector& p0,
        const Vector& p1,
        const Vector& p2,
        const Vector& p3,
        Vector& p4) -> bool
    {
        float denom = (p3.y - p2.y) * (p1.x - p0.x) - (p3.x - p2.x) * (p1.y - p0.y);
        float numea = (p3.x - p2.x) * (p0.y - p2.y) - (p3.y - p2.y) * (p0.x - p2.x);
        float numeb = (p1.x - p0.x) * (p0.y - p2.y) - (p1.y - p0.y) * (p0.x - p2.x);

        float denomAbs = Math::Abs(denom);
        float numeaAbs = Math::Abs(numea);
        float numebAbs = Math::Abs(numeb);

        if (numeaAbs < Math::epsilon && numebAbs < Math::epsilon && denomAbs < Math::epsilon)
        {
            p4 = Vector::Lerp(p0, p1, 0.5);
            return true;
        }

        if (denomAbs < Math::epsilon)
            return false;

        float mua = numea / denom;
        float mub = numeb / denom;

        if (mua < 0 || mua > 1 || mub < 0 || mub > 1) {
            return false;
        }

        float muax = numea / denom;
        p4 = (p1 - p0) * muax;
        p4 += p0;
        return true;
    };

    bool intersected = areLinesIntersected(c0, e0, c1, e1, i0);

    float vertices[] = {
        e0.x, e0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 0
        e1.x, e1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 1
        u0.x, u0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 2
        u1.x, u1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 3
        n0.x, n0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 4
        n1.x, n1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 5
        c0.x, c0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 6
        c1.x, c1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 7
        d0.x, d0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 8
        d1.x, d1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 9
        i0.x, i0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 10
        p1.x, p1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 11
    };

    uint indexLength = 21;
    uint indices[] = {
        2, 6, 8,
        8, 4, 2,
        7, 3, 5,
        5, 9, 7,
        6, 7, 11, // mid
        6, 0, 1,
        1, 7, 6,
    };

    if (intersected)
    {
        indexLength = 18;
        indices[15] = 6;
        indices[16] = 10;
        indices[17] = 7;
    }

    AddData(vertices, 12, indices, indexLength);
}

void Renderer::AddLine(const Vector& p0, const Vector& p1, float weight, const Color& c)
{
    Vector m0 = Vector::Lerp(p0, p1, 0.5);

    AddTerminator(p0, m0, weight, c);
    AddTerminator(p1, m0, weight, c);
}

void Renderer::AddPolyline(const std::vector<Vector>& ps, float w, const Color& c)
{
    if (ps.size() > 1)
    {
        std::vector<Vector> ms;

        for (size_t i = 0; i < ps.size() - 1; ++i)
            ms.push_back(Vector::Lerp(ps[i], ps[i + 1], 0.5));

        for (size_t i = 1; i < ms.size(); ++i)
            AddAnchor(ms[i - 1], ps[i], ms[i], w, c);

        AddTerminator(ps.front(), ms.front(), w, c);
        AddTerminator(ps.back(), ms.back(), w, c);
    }
}

void Renderer::AddPolyring(const std::vector<Vector>& ps, float w, const Color& c)
{
    if (ps.size() > 1)
    {
        std::vector<Vector> ms;

        for (size_t i = 0; i < ps.size() - 1; ++i)
            ms.push_back(Vector::Lerp(ps[i], ps[i + 1], 0.5));

        for (size_t i = 1; i < ms.size(); ++i)
            AddAnchor(ms[i - 1], ps[i], ms[i], w, c);

        Vector m = Vector::Lerp(ps.front(), ps.back(), 0.5);

        AddAnchor(m, ps.front(), ms.front(), w, c);
        AddAnchor(ms.back(), ps.back(), m, w, c);
    }
}

void Renderer::AddQuadraticBz(const Vector& p0, const Vector& p1, const Vector& p2, float w, const Color& c)
{
    auto GetLerped = [](const Vector& p0, const Vector& p1, const Vector& p2, float t)
    {
        Vector pt;

        float t2 = t * 2;
        float tq = t * t;
        float ti = 1.f - t;
        float tiq = ti * ti;

        pt.x = tiq * p0.x +
               ti * t2 * p1.x +
               tq * p2.x;
        pt.y = tiq * p0.y +
               ti * t2 * p1.y +
               tq * p2.y;

        return pt;
    };

    std::vector<Vector> points;

    for (int i = 0; i <= 10; ++i)
        points.push_back(GetLerped(p0, p1, p2, i / 10.f));

    AddPolyline(points, w, c);
}

void Renderer::AddQuadraticBzi(const Vector& p0, const Vector& p1, const Vector& p2, float w, const Color& c)
{
    Vector px = p1 * 2 - (p0 + p2) / 2;
    AddQuadraticBz(p0, px, p2, w, c);
}

void Renderer::AddQubicBz(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, float w, const Color& c)
{
    auto GetLerped = [](const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, float t)
    {
        Vector pt;

        float t3 = t * 3;
        float tc = t * t * t;
        float ti = 1.f - t;
        float tiq = ti * ti;
        float tic = ti * ti * ti;

        pt.x = tic * p0.x +
               tiq * t3 * p1.x +
               ti * t3 * t * p2.x +
               tc * p3.x;
        pt.y = tic * p0.y +
               tiq * t3 * p1.y +
               ti * t3 * t * p2.y +
               tc * p3.y;

        return pt;
    };

    std::vector<Vector> points;

    for (int i = 0; i <= 10; ++i)
        points.push_back(GetLerped(p0, p1, p2, p3, i/10.f));

    AddPolyline(points, w, c);
}

}
