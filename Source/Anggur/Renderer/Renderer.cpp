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

    static const usize length = 9;
};

Camera Camera::main;

Shader Renderer::batchShader;

float* Renderer::vertexData;
uint* Renderer::indexData;

usize Renderer::vertexCounter;
usize Renderer::indexCounter;

usize Renderer::maxQuad = 2048;
usize Renderer::circleSegment = 32;

usize Renderer::maxVertices;
usize Renderer::maxIndices;

usize Renderer::maxTextureUnits;
usize Renderer::textureCounter;
float Renderer::textureIndex;
int* Renderer::textureIndices;
Texture* Renderer::textureData;

Matrix Renderer::viewProjectionMatrix;

VertexArray Renderer::vertexArray;
VertexBuffer Renderer::vertexBuffer;
IndexBuffer Renderer::indexBuffer;

Shader& Renderer::GetBatchShader()
{
    return batchShader;
}

void Renderer::CreateBatchShader()
{
    batchShader.SetVertexSource(R"(
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
    maxTextureUnits = maxUnit;

    textureData = new Texture[maxTextureUnits];
    textureIndices = new int[maxTextureUnits];

    for (usize i = 0; i < maxTextureUnits; ++i)
    {
        textureIndices[i] = i;

        fragmentSource += "case " + std::to_string(i) + ":\n";
        fragmentSource += "aColor = texture(uTex[" + std::to_string(i) + "], vTexCoord) * vColor;\n";
        fragmentSource += "break;\n";

        fragmentSource += "case " + std::to_string(i + maxTextureUnits) + ":\n";
        fragmentSource += "aColor = vec4(1.f, 1.f, 1.f, texture(uTex[" + std::to_string(i) + "], vTexCoord)) * vColor;\n";
        fragmentSource += "break;\n";
    }

    fragmentSource += R"(
            }
        }
    )";

    batchShader.SetFragmentSource(fragmentSource);
    batchShader.Compile();

}

void Renderer::Initialize()
{
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CreateBatchShader();

    maxVertices = maxQuad * 4;
    maxIndices  = maxQuad * 6;

    vertexArray.Create();
    vertexArray.Bind();
    vertexData = new float[maxVertices * Vertex::length];
    indexData = new uint[maxIndices];

    vertexBuffer.Create();
    vertexBuffer.Bind();
    vertexBuffer.SetCapacity(sizeof(float) * maxVertices * Vertex::length);

    vertexArray.SetAttributePtr(0, 2, sizeof(Vertex), (void*) offsetof(Vertex, position));
    vertexArray.SetAttributePtr(1, 4, sizeof(Vertex), (void*) offsetof(Vertex, color));
    vertexArray.SetAttributePtr(2, 2, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));
    vertexArray.SetAttributePtr(3, 1, sizeof(Vertex), (void*) offsetof(Vertex, texIndex));

    indexBuffer.Create();
    indexBuffer.Bind();
    indexBuffer.SetCapacity(sizeof(uint) * maxIndices);

    viewProjectionMatrix = Matrix::identity;

    FlushData();
}

void Renderer::Terminate()
{
    delete[] vertexData;
    delete[] indexData;
    delete[] textureData;
    delete[] textureIndices;

    vertexData     = nullptr;
    indexData      = nullptr;
    textureData    = nullptr;
    textureIndices = nullptr;

    indexBuffer.Destroy();
    vertexBuffer.Destroy();
    vertexArray.Destroy();
    batchShader.Destroy();
}

void Renderer::SetViewport(Vector size)
{
    glViewport(0, 0, size.x, size.y);
}

void Renderer::SetMaxQuad(usize max)
{
    if (max < circleSegment) max = circleSegment;
    maxQuad = max;
}

void Renderer::SetCircleSegment(usize segment)
{
    if (segment < 3) segment = 3;
    if (maxQuad < segment) maxQuad = segment;
    circleSegment = segment;
}

void Renderer::ClearBackground(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Render()
{
    if (vertexCounter == 0)
        return;

    batchShader.Bind();
    batchShader.SetMatrix("uViewProjection", viewProjectionMatrix);
    batchShader.SetInt("uTex", maxTextureUnits, textureIndices);

    for (usize i = 0; i < textureCounter; ++i)
        textureData[i].Bind(i);

    vertexBuffer.Bind();
    vertexBuffer.SetData(sizeof(float) * maxVertices * Vertex::length, vertexData);

    indexBuffer.Bind();
    indexBuffer.SetData(sizeof(uint) * maxIndices, indexData);

    vertexArray.Bind();
    glDrawElements(GL_TRIANGLES, indexCounter, GL_UNSIGNED_INT, nullptr);

    FlushData();
}

void Renderer::SetViewProjectionMatrix(const Matrix& matrix)
{
    viewProjectionMatrix = matrix;
}

void Renderer::FlushData()
{
    vertexCounter  = 0;
    indexCounter   = 0;
    textureCounter = 0;
}

void Renderer::CheckLimit(usize vertexOffset, usize indexOffset, usize textureOffset)
{
    if (vertexCounter + vertexOffset > maxVertices) return Render();
    if (indexCounter + indexOffset > maxIndices) return Render();
    if (textureCounter + textureOffset > maxTextureUnits) return Render();
}

void Renderer::AddText(Font& font, const std::string& text, const Vector& p0, float size, const Color& c)
{
    float x = 0;
    float charSpacing = size * 0.05;
    float wordSpacing = size * 0.1;

    for (auto ch: text)
    {
        if (ch == 32)
            x += wordSpacing;

        const CharRect& cr = font.GetCharRect(ch);
        Vector p1(p0.x + x, p0.y);
        float x1 = cr.ratio * size;

        AddQuadx(p1,
                 {p1.x + x1, p1.y},
                 {p1.x + x1, p1.y + size},
                 {p1.x, p1.y + size},
                 {cr.texOffsetX, 0},
                 {cr.texOffsetX + cr.texClipX, 0},
                 {cr.texOffsetX + cr.texClipX, 1},
                 {cr.texOffsetX, 1}, font.GetTexture(), c);

        x += x1 + charSpacing;
    }
}

void Renderer::AddText(Font& font, int ch, const Vector& p0, float size, const Color& c)
{
    const CharRect& cr = font.GetCharRect(ch);
    float x1 = cr.ratio * size;

    AddQuadx(p0,
             {p0.x + x1, p0.y},
             {p0.x + x1, p0.y + size},
             {p0.x, p0.y + size},
             {cr.texOffsetX, 0},
             {cr.texOffsetX + cr.texClipX, 0},
             {cr.texOffsetX + cr.texClipX, 1},
             {cr.texOffsetX, 1}, font.GetTexture(), c);

}

void Renderer::AddData(const float* vetices, usize vertexLength, const uint* indices, usize indexLength)
{
    usize vertexOffset = vertexCounter * Vertex::length;
    usize vertexSize   = vertexLength * Vertex::length;

    for (usize i = 0; i < vertexSize; ++i)
        vertexData[vertexOffset + i] = vetices[i];

    for (usize i = 0; i < indexLength; ++i)
        indexData[indexCounter + i] = vertexCounter + indices[i];

    vertexCounter += vertexLength;
    indexCounter += indexLength;
}

void Renderer::AddDatax(const Texture& texture)
{
    for (usize i = 0; i < textureCounter; ++i)
    {
        if (textureData[i].GetId() == texture.GetId())
        {
            textureIndex = i;
            return;
        }
    }

    textureData[textureCounter] = texture;
    textureIndex = textureCounter;
    textureCounter++;
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

void Renderer::AddQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Color& c0, const Color& c1, const Color& c2, const Color& c3)
{
    CheckLimit(4, 6);

    float vertices[] = {
        p0.x, p0.y, c0.r, c0.g, c0.b, c0.a, 0.f, 0.f, -1,
        p1.x, p1.y, c1.r, c1.g, c1.b, c1.a, 0.f, 0.f, -1,
        p2.x, p2.y, c2.r, c2.g, c2.b, c2.a, 0.f, 0.f, -1,
        p3.x, p3.y, c3.r, c3.g, c3.b, c3.a, 0.f, 0.f, -1,
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
        p0,
        Vector(p0.x + w, p0.y),
        Vector(p0.x + w, p0.y + h),
        Vector(p0.x,     p0.y + h),
        c
    );
}

void Renderer::AddBox(const Vector& position, const Vector& radii, const Color& c)
{
    AddQuad(
        Vector(position.x - radii.x, position.y - radii.y),
        Vector(position.x + radii.x, position.y - radii.y),
        Vector(position.x + radii.x, position.y + radii.y),
        Vector(position.x - radii.x, position.y + radii.y),
        c
    );
}

void Renderer::AddConvex(const std::vector<Vector>& ps, const Color& c)
{
    usize triangles = ps.size() - 2;
    uint indices[triangles * 3];
    float vertices[ps.size() * Vertex::length];

    for (usize i = 0, offset = 0; i < ps.size(); ++i)
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

    for (usize i = 0, offset = 0; i < triangles; ++i)
    {
        indices[offset + 0] = 0;
        indices[offset + 1] = i + 1;
        indices[offset + 2] = i + 2;

        offset += 3;
    }

    AddData(vertices, ps.size(), indices, triangles * 3);
}

void Renderer::AddPolygon(const Vector& p0, float r, usize segments, const Color& c)
{
    if (segments < 3) segments = 3;

    usize triangles = segments - 2;

    CheckLimit(segments, triangles * 3);

    float theta           = Math::twoPi / segments;
    float tangetialFactor = Math::Tan(theta);
    float radialFactor    = Math::Cos(theta);

    float x = r;
    float y = 0;

    float vertices[segments * Vertex::length];
    uint indices[triangles * 3];

    for (usize i = 0, offset = 0; i < segments; i++)
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

    for (usize i = 0, offset = 0; i < triangles; ++i)
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
    AddPolygon(p0, r, circleSegment, c);
}

void Renderer::AddTriangle(const Vector& p0, const Vector& p1, const Vector& p2, const Transform& f, const Color& c)
{
    Matrix m = f.ToMatrix();
    AddTriangle(p0 * m, p1 * m, p2 * m, c);
}

void Renderer::AddQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Transform& f, const Color& c)
{
    Matrix m = f.ToMatrix();
    AddQuad(p0 * m, p1 * m, p2 * m, p3 * m, c);
}

void Renderer::AddRect(const Vector& p0, float w, float h, const Transform& f, const Color& c)
{
    Matrix m = f.ToMatrix();
    AddQuad(
        p0 * m,
        Vector(p0.x + w, p0.y) * m,
        Vector(p0.x + w, p0.y + h) * m,
        Vector(p0.x,     p0.y + h) * m,
        c
    );
}

void Renderer::AddBox(const Vector& p0, const Vector& r, const Transform& f, const Color& c)
{
    Matrix m = f.ToMatrix();
    AddQuad(
        Vector(p0.x - r.x, p0.y - r.y) * m,
        Vector(p0.x + r.x, p0.y - r.y) * m,
        Vector(p0.x + r.x, p0.y + r.y) * m,
        Vector(p0.x - r.x, p0.y + r.y) * m,
        c
    );
}

void Renderer::AddPolygon(const Vector& p0, float r, usize segments, const Transform& f, const Color& c)
{
    AddPolygon(p0 * f.ToMatrix(), r, segments, c);
}

void Renderer::AddCircle(const Vector& p0, float r, const Transform& f, const Color& c)
{
    AddCircle(p0 * f.ToMatrix(), r, c);
}


void Renderer::AddQuadx(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Vector& t0, const Vector& t1, const Vector& t2, const Vector& t3, const Texture& t, const Color& c)
{
    CheckLimit(4, 6, 1);

    AddDatax(t);
    float ti = textureIndex;
    if (t.GetChannels() == 1)
        ti += maxTextureUnits;

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

void Renderer::AddBoxx(const Vector& position, const Vector& radii, const Texture& t, const Color& c)
{
    AddQuadx(
        {position.x - radii.x, position.y - radii.y},
        {position.x + radii.x, position.y - radii.y},
        {position.x + radii.x, position.y + radii.y},
        {position.x - radii.x, position.y + radii.y},
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1},
        t,
        c
    );
}

void Renderer::Addx(const Vector& p0, const Texture& t, const Transform& f, const Color& c)
{
    Addx(p0 * f.ToMatrix(), t, c);;
}

void Renderer::AddQuadx(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, const Vector& t0, const Vector& t1, const Vector& t2, const Vector& t3, const Texture& t, const Transform& f, const Color& c)
{
    Matrix m = f.ToMatrix();
    AddQuadx(p0 * m, p1 * m, p2 * m, p3 * m, t0, t1, t2, t3, t, c);
}

void Renderer::AddRectx(const Vector& p0, float w, float h, const Texture& t, const Transform& f, const Color& c)
{
    AddRectx(p0 * f.ToMatrix(), w, h, t, c);
}

void Renderer::AddBoxx(const Vector& p0, const Vector& radii, const Texture& t, const Transform& f, const Color& c)
{
    AddBoxx(p0 * f.ToMatrix(), radii, t, c);
}

void Renderer::AddTerminator(const Vector& p0, const Vector& p1, float weight, const Color& c)
{
    Vector p3 = (p0 - p1).Normalize() * weight;
    Vector t0 = (p1 - p0).GetPerpen().Normalize() * weight;

    AddQuad(p1 + t0, p3 + p0 + t0, p3 + p0 - t0, p1 - t0, c);
}

void Renderer::AddAnchor(const Vector& p0, const Vector& p1, const Vector& p2, float w0, const Color& c)
{
    Vector t0 = (p1 - p0).GetPerpen();
    Vector t2 = (p2 - p1).GetPerpen();

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

        for (usize i = 0; i < ps.size() - 1; ++i)
            ms.push_back(Vector::Lerp(ps[i], ps[i + 1], 0.5));

        for (usize i = 1; i < ms.size(); ++i)
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

        for (usize i = 0; i < ps.size() - 1; ++i)
            ms.push_back(Vector::Lerp(ps[i], ps[i + 1], 0.5));

        for (usize i = 1; i < ms.size(); ++i)
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
