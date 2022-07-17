#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Math/Vector2.h>
#include "CanvasRenderer.h"
#include <vector>
#include <iostream>

namespace Anggur 
{
    CanvasRenderer::CanvasRenderer() 
    {
        InitializeVertexPool();
        InitializeTexturePool();
        InitializeShader();
    }

    CanvasRenderer::~CanvasRenderer()
    {
    }

    void CanvasRenderer::InitializeVertexPool() 
    {
        vertices.assign(batchVertex, CanvasVertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
        indices.assign(batchVertex * batchIndexMultiplier, 0);

        vertexBuffer = std::make_shared<VertexBuffer>();
        vertexBuffer->SetCapacity(sizeof(CanvasVertex) * vertices.size());

        vertexArray = std::make_shared<VertexArray>();
        vertexArray->SetAttribute(0, 2, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, position));
        vertexArray->SetAttribute(1, 4, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, color));
        vertexArray->SetAttribute(2, 2, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, uv));
        vertexArray->SetAttribute(3, 1, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, slot));
        
        indexBuffer = std::make_shared<IndexBuffer>();
        indexBuffer->SetCapacity(sizeof(uint32_t) * indices.size());
    }

    void CanvasRenderer::InitializeTexturePool() 
    {
        textures.assign(Texture::GetMaxSlot(), nullptr);
        slots.reserve(Texture::GetMaxSlot());

        for (int i = 0; i < Texture::GetMaxSlot(); ++i) 
        {
            slots.push_back(i);
        }

        uint8_t whitePixel[] = {255, 255, 255, 255};
        whiteTexture = std::make_shared<Texture2D>(whitePixel, 1, 1, 4);
    }

    void CanvasRenderer::InitializeShader() 
    {
        shader = std::make_shared<Shader>();
        shader->SetVertexSource(R"(
            #version 330 core

            layout (location = 0) in vec2 aPosition;
            layout (location = 1) in vec4 aColor;
            layout (location = 2) in vec2 aUV;
            layout (location = 3) in float aSlot;

            out vec4 vColor;
            out vec2 vUV;
            out float vSlot;

            uniform mat3 uViewProjection;

            void main() {
                gl_Position = vec4(uViewProjection * vec3(aPosition, 1.0f), 1.0f);

                vColor = aColor;
                vUV = aUV;
                vSlot = aSlot;
            }
        )");

        shader->SetFragmentSource(R"(
            #version 330 core
            
            in vec4 vColor;
            in vec2 vUV;
            in float vSlot;

            out vec4 fColor;

            uniform sampler2D uSlots[)" + std::to_string(Texture::GetMaxSlot()) + R"(];
            
            void main() {
                fColor = texture(uSlots[int(vSlot)], vUV) * vColor;
            }
        )");

        shader->Compile();
    }

    void CanvasRenderer::SetBatchChunk(size_t vertex, size_t indexMultiplier) 
    {
        batchVertex = vertex;
        batchIndexMultiplier = indexMultiplier;
    }

    void CanvasRenderer::Clear(const Vector4& color) 
    {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

    void CanvasRenderer::SetViewport(const Vector2& size) 
    {
        glViewport(0, 0, size.x, size.y);
    }

    void CanvasRenderer::SetViewport(const Vector2& position, const Vector2& size) 
    {
        glViewport(position.x, position.y, size.x, size.y);
    }

    void CanvasRenderer::SetViewProjection(const Matrix3& newViewProjection)
    {
        viewProjection = newViewProjection;
    }

    void CanvasRenderer::Begin() 
    {
        renderCount = 0;

        FlushData();
    }

    void CanvasRenderer::Begin(const Matrix3& viewProjection) 
    {
        SetViewProjection(viewProjection);
        Begin();
    }

    void CanvasRenderer::End() 
    {
        Flush();
    }

    void CanvasRenderer::Flush() 
    {
        if (vertexOffset == 0) 
        {
            return;
        }

        for (size_t i = 0; i < textureOffset; ++i) 
        {
            textures[i]->Bind(i);
        }

        shader->Bind();
        shader->SetUniformMatrix3("uViewProjection", viewProjection);
        shader->SetUniformInt("uSlots", textureOffset, slots.data());

        vertexArray->Bind();
        
        vertexBuffer->Bind();
        vertexBuffer->SetData(sizeof(CanvasVertex) * vertexOffset, vertices.data());

        indexBuffer->Bind();
        indexBuffer->SetData(sizeof(uint32_t) * indexOffset, indices.data());

        glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

        FlushData();

        ++renderCount;
    }

    void CanvasRenderer::FlushData() 
    {
        vertexOffset = 0;
        indexOffset = 0;
        textureOffset = 0;
    }

    bool CanvasRenderer::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) 
    {
        return 
            vertexOffset + newVertexSize > vertices.size() ||
            indexOffset + newIndexSize > indices.size() ||
            textureOffset + newTextureSize > textures.size();
    }

    void CanvasRenderer::Draw(const std::vector<CanvasVertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) 
    {
        if (IsCapacityMaxout(newVertices.size(), newIndices.size(), 1)) 
        {
            Flush();
        }

        // Find or add new texture slot 
        int textureSlot = 0;

        // This code only create one branch
        for (; textureSlot < textureOffset && textures[textureSlot]->GetID() != texture->GetID(); ++textureSlot);

        if (textureSlot == textureOffset) 
        {
            textureSlot = textureOffset;
            textures[textureOffset] = texture;
            textureOffset += 1;
        }

        for (size_t i = 0; i < newVertices.size(); ++i) 
        {
            auto& vertex = vertices[i + vertexOffset];

            vertex = newVertices[i];
            vertex.slot = textureSlot;
        }

        for (size_t i = 0; i < newIndices.size(); ++i) 
        {
            indices[i + indexOffset] = newIndices[i] + vertexOffset;
        }

        vertexOffset += newVertices.size();
        indexOffset += newIndices.size();
    }

    // 2D primitives

    void CanvasRenderer::DrawTriangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector4& color) 
    {
        Draw(
            { 
                CanvasVertex(model * point0, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point1, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point2, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
            }, {
                0, 1, 2,
            }, 
            whiteTexture
        );
    }

    void CanvasRenderer::DrawQuad(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector4& color) 
    {
        Draw(
            {     
                CanvasVertex(model * point0, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point1, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point2, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point3, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
            }, {
                0, 1, 2,
                2, 3, 0,
            }, 
            whiteTexture
        );
    }


    void CanvasRenderer::DrawRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector4& color) 
    {
        Draw(
            {    
                CanvasVertex(model * point0,                      Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * Vector2(point1.x, point0.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point1,                      Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * Vector2(point0.x, point1.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
            }, {
                0, 1, 2,
                2, 3, 0,
            }, 
            whiteTexture
        );
    }

    void CanvasRenderer::DrawTexturedRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePoint0, const Vector2& texturePoint1, const Vector4& color)
    {
        Draw(
            {    
                CanvasVertex(model * point0,                      Vector4(color.x, color.y, color.z, color.w), texturePoint0),
                CanvasVertex(model * Vector2(point1.x, point0.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePoint1.x, texturePoint0.y)),
                CanvasVertex(model * point1,                      Vector4(color.x, color.y, color.z, color.w), texturePoint1),
                CanvasVertex(model * Vector2(point0.x, point1.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePoint0.x, texturePoint1.y)),
            }, {
                0, 1, 2,
                2, 3, 0,
            }, 
            texture
        );
    }

    void CanvasRenderer::DrawCircle(const Matrix3& model, float radius, int segment, const Vector4& color)
    {
        if (segment < 3)
            segment = 3;

        Vector2 position(0, 0);
        size_t triangleSize = segment - 2;

        float theta = (Math::twoPi / segment);
        float tangetialFactor = Math::tan(theta);
        float radialFactor = Math::cos(theta);

        float x = radius;
        float y = 0;

        std::vector<CanvasVertex> vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(segment);
        indices.reserve(triangleSize * 3);

        for (size_t i = 0; i < segment; i++) 
        {
            CanvasVertex vertex;
            vertex.position = model * Vector2(position.x + x, position.y + y);
            // vertex.textureCoord = Vector2(x / length, y / length);
            vertex.color = color;

            float tx = -y;
            float ty = x;

            x += tx * tangetialFactor;
            y += ty * tangetialFactor;

            x *= radialFactor;
            y *= radialFactor;

            vertices.push_back(std::move(vertex));
        }

        for (size_t i = 0, offset = 0; i < triangleSize; ++i) 
        {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
            offset += 3;
        }

        Draw(vertices, indices, whiteTexture);
    }


    void CanvasRenderer::DrawArc(const Matrix3& model, float radius, float beginAngle, float sweepAngle, int segment, const Vector4& color)
    {
        if (segment < 3)
            segment = 3;

        Vector2 position(0, 0);

        // // TODO: simplify this equation
        float theta = Math::twoPi / ((segment - 1) / (sweepAngle / Math::twoPi));

        float tangetialFactor = Math::tan(theta);
        float radialFactor = Math::cos(theta);

        Vector2 walker = Vector2::createPolar(radius, beginAngle);

        std::vector<CanvasVertex> vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(segment + 1);
        indices.reserve(segment);

        CanvasVertex vertex;
        vertex.position = model * position;
        vertex.color = color;
        vertices.push_back(vertex);

        for (size_t i = 0; i < segment; i++) 
        {
            CanvasVertex vertex;
            vertex.position = model * Vector2(position.x + walker.x, position.y + walker.y);
            vertex.color = color;

            float tx = -walker.y;
            float ty = walker.x;

            walker.x += tx * tangetialFactor;
            walker.y += ty * tangetialFactor;

            walker.x *= radialFactor;
            walker.y *= radialFactor;

            vertices.push_back(std::move(vertex));
        }

        for (size_t i = 0; i < segment - 1; ++i) 
        {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }

        indices.push_back(0);
        indices.push_back(segment);
        indices.push_back(1);

        Draw(vertices, indices, whiteTexture);
    }
}