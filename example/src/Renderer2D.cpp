#include "Renderer2D.h"
#include <Anggur/Math/Matrix3.h>
#include <iostream>

Renderer2D::Renderer2D() 
{
    InitializeVertexPool();
    InitializeTexturePool();
    InitializeShader();
}

Renderer2D::~Renderer2D() 
{
}

void Renderer2D::InitializeVertexPool() 
{
    vertices.assign(batchVertex, Vertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
    indices.assign(batchVertex * batchIndexMultiplier, 0);

    vertexBuffer.SetCapacity(sizeof(Vertex) * vertices.size());

    vertexArray.SetAttribute(0, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    vertexArray.SetAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
    vertexArray.SetAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
    vertexArray.SetAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texSlot));
    
    indexBuffer.SetCapacity(sizeof(uint32_t) * indices.size());
}

void Renderer2D::InitializeTexturePool() 
{
    textures.assign(Texture2D::GetMaxSlot(), nullptr);
}

void Renderer2D::InitializeShader() 
{
    shader.SetVertexSource(R"(
        #version 330 core

        layout (location = 0) in vec2 aPosition;
        layout (location = 1) in vec4 aColor;
        layout (location = 2) in vec2 aTexCoord;
        layout (location = 3) in float aTexSlot;

        out vec4 vColor;
        out vec2 vTexCoord;
        out float vTexSlot;

        uniform mat3 uViewProjection;

        void main() {
            gl_Position = vec4(uViewProjection * vec3(aPosition, 1.0f), 1.0f);

            vColor = aColor;
            vTexCoord = aTexCoord;
            vTexSlot = aTexSlot;
        }
    )");

    shader.SetFragmentSource(R"(
        #version 330 core
        
        in vec4 vColor;
        in vec2 vTexCoord;
        in float vTexSlot;

        out vec4 fColor;

        uniform sampler2D uTextures[)" + std::to_string(Texture2D::GetMaxSlot()) + R"(];
        
        void main() {
            // fColor = vColor;
            fColor = texture(uTextures[int(vTexSlot)], vTexCoord);
        }
    )");

    shader.Compile();
}

void Renderer2D::SetClearColor(const Vector4& color) 
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer2D::SetBatchChunk(size_t vertex, size_t indexMultiplier) 
{
    batchVertex = vertex;
    batchIndexMultiplier = indexMultiplier;
}

void Renderer2D::Clear() 
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::SetViewProjection(const Matrix3& newViewProjection) 
{
    viewProjection = newViewProjection;
}

void Renderer2D::Begin() 
{
    renderCount = 0;
}

void Renderer2D::End() 
{
    Renderer2D::Flush();
}

void Renderer2D::Flush() 
{
    if (vertexOffset == 0) {
        return;
    }

    shader.Bind();
    shader.SetUniformMatrix3("uViewProjection", viewProjection);

    vertexArray.Bind();
    
    vertexBuffer.Bind();
    vertexBuffer.SetData(sizeof(Vertex) * vertices.size(), vertices.data());

    indexBuffer.Bind();
    indexBuffer.SetData(sizeof(uint32_t) * indices.size(), indices.data());

    for (size_t i = 0; i < textureOffset; ++i) 
    {
        textures[i]->Bind();
    }

    glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

    vertexOffset = 0;
    indexOffset = 0;
	textureOffset = 0;

    textures.clear();

    ++renderCount;
}

bool Renderer2D::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) 
{
    return 
        vertexOffset + newVertexSize > vertices.size() ||
        indexOffset + newIndexSize > indices.size() ||
        textureOffset + newTextureSize > textures.size();
}

void Renderer2D::Render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) 
{
    if (IsCapacityMaxout(newVertices.size(), newIndices.size(), 1)) 
    {
        Flush();
    }

    for (size_t i = 0; i < newVertices.size(); ++i) 
    {
        auto& vertex = vertices[i + vertexOffset];

        vertex = newVertices[i];
        vertex.texSlot = textureOffset;
    }

    for (size_t i = 0; i < newIndices.size(); ++i) 
    {
        indices[i + indexOffset] = newIndices[i] + vertexOffset;
    }

    // TODO: naive method add cheking same texture ID or something
    textures[textureOffset] = texture;

    vertexOffset += newVertices.size();
    indexOffset += newIndices.size();
    textureOffset += 1;
}

void Renderer2D::RenderRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition, const Vector2& textureSize, const Vector4& color) 
{
    Render(
        {
            // position                                               // color                                     // texCoord          // texSlot
            Vertex(Vector2(position.x,          position.y),          Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x,                 texturePosition.y                )),
            Vertex(Vector2(position.x + size.x, position.y),	      Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y                )),
            Vertex(Vector2(position.x,          position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x,                 texturePosition.y + textureSize.y)),
            Vertex(Vector2(position.x + size.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y)),
        }, 
        {
            0, 1, 2,
            2, 3, 1,
        }, 
        texture
    );
}
