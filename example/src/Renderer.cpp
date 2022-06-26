#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Math/Matrix4.h>
#include "Renderer.h"
#include <vector>
#include <iostream>

namespace Anggur 
{
    struct RendererData 
    {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;

        std::vector<Renderer::Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;
        std::vector<int> textureSlots;

        std::shared_ptr<Texture2D> whiteTexture;

        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        size_t textureOffset = 0;

        size_t renderCount = 0;
        size_t batchVertex = 128;
        size_t batchIndexMultiplier = 2;

        Matrix4 viewProjection;
    };

    static RendererData rendererData;

    Renderer::Renderer() 
    {
    }

    void Renderer::Initialize() 
    {
        InitializeVertexPool();
        InitializeTexturePool();
        InitializeShader();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::InitializeVertexPool() 
    {
        rendererData.vertices.assign(rendererData.batchVertex, Vertex(Vector3(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
        rendererData.indices.assign(rendererData.batchVertex * rendererData.batchIndexMultiplier, 0);

        rendererData.vertexBuffer = std::make_shared<VertexBuffer>();
        rendererData.vertexBuffer->SetCapacity(sizeof(Vertex) * rendererData.vertices.size());

        rendererData.vertexArray = std::make_shared<VertexArray>();
        rendererData.vertexArray->SetAttribute(0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
        rendererData.vertexArray->SetAttribute(1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
        rendererData.vertexArray->SetAttribute(2, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
        rendererData.vertexArray->SetAttribute(3, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureCoord));
        rendererData.vertexArray->SetAttribute(4, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureSlot));
        
        rendererData.indexBuffer = std::make_shared<IndexBuffer>();
        rendererData.indexBuffer->SetCapacity(sizeof(uint32_t) * rendererData.indices.size());
    }

    void Renderer::InitializeTexturePool() 
    {
        rendererData.textures.assign(Texture::GetMaxSlot(), nullptr);
        rendererData.textureSlots.reserve(Texture::GetMaxSlot());

        for (int i = 0; i < Texture::GetMaxSlot(); ++i) 
        {
            rendererData.textureSlots.push_back(i);
        }

        uint8_t whitePixel[] = {255, 255, 255, 255};
        rendererData.whiteTexture = std::make_shared<Texture2D>(whitePixel, 1, 1, 4);
    }

    void Renderer::InitializeShader() 
    {
        rendererData.shader = std::make_shared<Shader>();
        rendererData.shader->SetVertexSource(R"(
            #version 330 core

            layout (location = 0) in vec3 aPosition;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec4 aColor;
            layout (location = 3) in vec2 aTexCoord;
            layout (location = 4) in float aTexSlot;

            out vec4 vColor;
            out vec2 vTexCoord;
            out float vTexSlot;

            uniform mat4 uViewProjection;

            void main() {
                gl_Position = uViewProjection * vec4(aPosition, 1.0f);

                vColor = aColor;
                vTexCoord = aTexCoord;
                vTexSlot = aTexSlot;
            }
        )");

        rendererData.shader->SetFragmentSource(R"(
            #version 330 core
            
            in vec4 vColor;
            in vec2 vTexCoord;
            in float vTexSlot;

            out vec4 fColor;

            uniform sampler2D uTextures[)" + std::to_string(Texture::GetMaxSlot()) + R"(];
            
            void main() {
                fColor = texture(uTextures[int(vTexSlot)], vTexCoord) * vColor;
            }
        )");

        rendererData.shader->Compile();
    }

    void Renderer::SetBatchChunk(size_t vertex, size_t indexMultiplier) 
    {
        rendererData.batchVertex = vertex;
        rendererData.batchIndexMultiplier = indexMultiplier;
    }

    void Renderer::Clear(const Vector4& color) 
    {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetViewport(const Vector2& size) 
    {
        glViewport(0, 0, size.x, size.y);
    }

    void Renderer::SetViewport(const Vector2& position, const Vector2& size) 
    {
        glViewport(position.x, position.y, size.x, size.y);
    }

    void Renderer::SetViewProjection(const Matrix4& newViewProjection)
    {
        rendererData.viewProjection = newViewProjection;
    }

    void Renderer::Begin() 
    {
        rendererData.renderCount = 0;

        FlushData();
    }

    void Renderer::Begin(const Matrix4& viewProjection) 
    {
        SetViewProjection(viewProjection);
        Begin();
    }

    void Renderer::End() 
    {
        Flush();
    }

    void Renderer::Flush() 
    {
        if (rendererData.vertexOffset == 0) 
        {
            return;
        }

        for (size_t i = 0; i < rendererData.textureOffset; ++i) 
        {
            rendererData.textures[i]->Bind(i);
        }

        rendererData.shader->Bind();
        rendererData.shader->SetUniformMatrix4("uViewProjection", rendererData.viewProjection);
        rendererData.shader->SetUniformInt("uTextures", rendererData.textureOffset, rendererData.textureSlots.data());

        rendererData.vertexArray->Bind();
        
        rendererData.vertexBuffer->Bind();
        rendererData.vertexBuffer->SetData(sizeof(Vertex) * rendererData.vertexOffset, rendererData.vertices.data());

        rendererData.indexBuffer->Bind();
        rendererData.indexBuffer->SetData(sizeof(uint32_t) * rendererData.indexOffset, rendererData.indices.data());

        glDrawElements(GL_TRIANGLES, rendererData.indexOffset, GL_UNSIGNED_INT, nullptr);

        for (size_t i = 0; i < rendererData.vertexOffset; ++i) 
        {
            Vertex& vertex = rendererData.vertices[i];
            std::printf("%.2f, %.2f, %.2f\n", vertex.position.x, vertex.position.y, vertex.position.z);
        }
        std::cout << "\n";

        FlushData();

        ++rendererData.renderCount;
    }

    void Renderer::FlushData() 
    {
        rendererData.vertexOffset = 0;
        rendererData.indexOffset = 0;
        rendererData.textureOffset = 0;
    }

    bool Renderer::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) 
    {
        return 
            rendererData.vertexOffset + newVertexSize > rendererData.vertices.size() ||
            rendererData.indexOffset + newIndexSize > rendererData.indices.size() ||
            rendererData.textureOffset + newTextureSize > rendererData.textures.size();
    }

    void Renderer::Render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) 
    {
        if (IsCapacityMaxout(newVertices.size(), newIndices.size(), 1)) 
        {
            Flush();
        }

        // Find or add new texture slot 
        int textureSlot = 0;

        // This code only create one branch
        for (; textureSlot < rendererData.textureOffset && rendererData.textures[textureSlot]->GetID() != texture->GetID(); ++textureSlot);

        if (textureSlot == rendererData.textureOffset) 
        {
            textureSlot = rendererData.textureOffset;
            rendererData.textures[rendererData.textureOffset] = texture;
            rendererData.textureOffset += 1;
        }

        for (size_t i = 0; i < newVertices.size(); ++i) 
        {
            auto& vertex = rendererData.vertices[i + rendererData.vertexOffset];

            vertex = newVertices[i];
            vertex.textureSlot = textureSlot;
        }

        for (size_t i = 0; i < newIndices.size(); ++i) 
        {
            rendererData.indices[i + rendererData.indexOffset] = newIndices[i] + rendererData.vertexOffset;
        }

        rendererData.vertexOffset += newVertices.size();
        rendererData.indexOffset += newIndices.size();
    }

    void Renderer::RenderRectangle(const Transform& transform, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition, const Vector2& textureSize, const Vector4& color) 
    {
        Matrix4 model = transform.ToMatrix4();

        Render(
            {
                // position                                 // normal                  // color                                     // texCoord         
                Vertex(Vector3(-0.5f, -0.5f, 0.0f) * model, Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x, texturePosition.y)),
                Vertex(Vector3( 0.5f, -0.5f, 0.0f) * model, Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y)),
                Vertex(Vector3( 0.5f,  0.5f, 0.0f) * model, Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y)),
                Vertex(Vector3(-0.5f,  0.5f, 0.0f) * model, Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x, texturePosition.y + textureSize.y)),
            }, {
                0, 1, 2,
                2, 3, 0,
            }, 
            texture
        );
    }

    void Renderer::RenderPolygon(const Vector2& position, int segment, float length, float angle, const Vector4& color)
    {
        if (segment < 3)
            segment = 3;

        size_t triangleSize = segment - 2;

        float theta = (Math::twoPi / segment);
        float tangetialFactor = Math::Tan(theta);
        float radialFactor = Math::Cos(theta);

        float x = length;
        float y = 0;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(segment);
        indices.reserve(triangleSize * 3);

        for (size_t i = 0; i < segment; i++) 
        {
            Vertex vertex;
            vertex.position = Vector3(position.x + x, position.y + y);
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

        Render(vertices, indices, rendererData.whiteTexture);
    }

    void Renderer::RenderCircle(const Vector2& position, float length, const Vector4& color)
    {
        RenderPolygon(position, length * 0.025, length, 0.0f, color);
    }
}