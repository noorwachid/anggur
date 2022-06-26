#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Math/Matrix3.h>
#include "Renderer2D.h"
#include <vector>
#include <iostream>

struct Renderer2DData 
{
	std::shared_ptr<Shader> shader;
	std::shared_ptr<VertexArray> vertexArray;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	std::vector<Renderer2D::Vertex> vertices;
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

	Matrix3 viewProjection;
};

static Renderer2DData renderer2DData;

Renderer2D::Renderer2D() 
{
}

void Renderer2D::Initialize() 
{
    InitializeVertexPool();
    InitializeTexturePool();
    InitializeShader();
}

void Renderer2D::InitializeVertexPool() 
{
    renderer2DData.vertices.assign(renderer2DData.batchVertex, Vertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
    renderer2DData.indices.assign(renderer2DData.batchVertex * renderer2DData.batchIndexMultiplier, 0);

    renderer2DData.vertexBuffer = std::make_shared<VertexBuffer>();
    renderer2DData.vertexBuffer->SetCapacity(sizeof(Vertex) * renderer2DData.vertices.size());

    renderer2DData.vertexArray = std::make_shared<VertexArray>();
    renderer2DData.vertexArray->SetAttribute(0, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    renderer2DData.vertexArray->SetAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
    renderer2DData.vertexArray->SetAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureCoord));
    renderer2DData.vertexArray->SetAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureSlot));
    
    renderer2DData.indexBuffer = std::make_shared<IndexBuffer>();
    renderer2DData.indexBuffer->SetCapacity(sizeof(uint32_t) * renderer2DData.indices.size());
}

void Renderer2D::InitializeTexturePool() 
{
    renderer2DData.textures.assign(Texture::GetMaxSlot(), nullptr);
    renderer2DData.textureSlots.reserve(Texture::GetMaxSlot());

    for (int i = 0; i < Texture::GetMaxSlot(); ++i) 
    {
        renderer2DData.textureSlots.push_back(i);
    }

    uint8_t whitePixel[] = {255, 255, 255, 255};
    renderer2DData.whiteTexture = std::make_shared<Texture2D>(whitePixel, 1, 1, 4);
}

void Renderer2D::InitializeShader() 
{
    renderer2DData.shader = std::make_shared<Shader>();
    renderer2DData.shader->SetVertexSource(R"(
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

    renderer2DData.shader->SetFragmentSource(R"(
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

    renderer2DData.shader->Compile();
}

void Renderer2D::SetBatchChunk(size_t vertex, size_t indexMultiplier) 
{
    renderer2DData.batchVertex = vertex;
    renderer2DData.batchIndexMultiplier = indexMultiplier;
}

void Renderer2D::Clear(const Vector4& color) 
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::SetViewport(const Vector2& size) 
{
    glViewport(0, 0, size.x, size.y);
}

void Renderer2D::SetViewport(const Vector2& position, const Vector2& size) 
{
    glViewport(position.x, position.y, size.x, size.y);
}

void Renderer2D::SetViewProjection(const Matrix3& newViewProjection)
{
    renderer2DData.viewProjection = newViewProjection;
}

void Renderer2D::Begin() 
{
    renderer2DData.renderCount = 0;

    FlushData();
}

void Renderer2D::Begin(const Matrix3& viewProjection) 
{
    SetViewProjection(viewProjection);
    Begin();
}

void Renderer2D::End() 
{
    Flush();
}

void Renderer2D::Flush() 
{
    if (renderer2DData.vertexOffset == 0) 
    {
        return;
    }

    for (size_t i = 0; i < renderer2DData.textureOffset; ++i) 
    {
        renderer2DData.textures[i]->Bind(i);
    }

    renderer2DData.shader->Bind();
    renderer2DData.shader->SetUniformMatrix3("uViewProjection", renderer2DData.viewProjection);
    renderer2DData.shader->SetUniformInt("uTextures", renderer2DData.textureOffset, renderer2DData.textureSlots.data());

    renderer2DData.vertexArray->Bind();
    
    renderer2DData.vertexBuffer->Bind();
    renderer2DData.vertexBuffer->SetData(sizeof(Vertex) * renderer2DData.vertexOffset, renderer2DData.vertices.data());

    renderer2DData.indexBuffer->Bind();
    renderer2DData.indexBuffer->SetData(sizeof(uint32_t) * renderer2DData.indexOffset, renderer2DData.indices.data());

    glDrawElements(GL_TRIANGLES, renderer2DData.indexOffset, GL_UNSIGNED_INT, nullptr);

    FlushData();

    ++renderer2DData.renderCount;
}

void Renderer2D::FlushData() 
{
    renderer2DData.vertexOffset = 0;
    renderer2DData.indexOffset = 0;
	renderer2DData.textureOffset = 0;
}

bool Renderer2D::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) 
{
    return 
        renderer2DData.vertexOffset + newVertexSize > renderer2DData.vertices.size() ||
        renderer2DData.indexOffset + newIndexSize > renderer2DData.indices.size() ||
        renderer2DData.textureOffset + newTextureSize > renderer2DData.textures.size();
}

void Renderer2D::Render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) 
{
    if (IsCapacityMaxout(newVertices.size(), newIndices.size(), 1)) 
    {
        Flush();
    }

    // Find or add new texture slot 
    int textureSlot = 0;

    // This code only create one branch
    for (; textureSlot < renderer2DData.textureOffset && renderer2DData.textures[textureSlot]->GetID() != texture->GetID(); ++textureSlot);

    if (textureSlot == renderer2DData.textureOffset) 
    {
        textureSlot = renderer2DData.textureOffset;
        renderer2DData.textures[renderer2DData.textureOffset] = texture;
        renderer2DData.textureOffset += 1;
    }

    for (size_t i = 0; i < newVertices.size(); ++i) 
    {
        auto& vertex = renderer2DData.vertices[i + renderer2DData.vertexOffset];

        vertex = newVertices[i];
        vertex.textureSlot = textureSlot;
    }

    for (size_t i = 0; i < newIndices.size(); ++i) 
    {
        renderer2DData.indices[i + renderer2DData.indexOffset] = newIndices[i] + renderer2DData.vertexOffset;
    }

    renderer2DData.vertexOffset += newVertices.size();
    renderer2DData.indexOffset += newIndices.size();
}

void Renderer2D::RenderRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition, const Vector2& textureSize, const Vector4& color) 
{
    Render(
        {
            // position                                               // color                                     // texCoord         
            Vertex(Vector2(position.x,          position.y),          Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x,                 texturePosition.y                )),
            Vertex(Vector2(position.x + size.x, position.y),	      Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y                )),
            Vertex(Vector2(position.x,          position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x,                 texturePosition.y + textureSize.y)),
            Vertex(Vector2(position.x + size.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y)),
        }, {
            0, 1, 2,
            2, 3, 1,
        }, 
        texture
    );
}

void Renderer2D::RenderRectangle(const Vector2& position, const Vector2& size, const Vector4& color) 
{
    RenderRectangle(position, size, renderer2DData.whiteTexture, Vector2::zero, Vector2::one, color);
}

void Renderer2D::RenderPolygon(const Vector2& position, int segment, float length, float angle, const Vector4& color)
{
    if (segment < 3)
        segment = 3;

    size_t triangleSize = segment - 2;

    float theta = Math::twoPi / segment;
    float tangetialFactor = Math::Tan(theta);
    float radialFactor = Math::Cos(theta);

    float x = length;
    float y = 0;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    Vertex vertex;

    vertices.assign(segment, vertex);
    indices.assign(triangleSize * 3, 0);

    for (size_t i = 0, offset = 0; i < segment; i++) 
    {
        vertices[offset].position = Vector2(position.x + x, position.y + y);
        vertices[offset].color = color;

        float tx = -y;
        float ty = x;

        x += tx * tangetialFactor;
        y += ty * tangetialFactor;

        x *= radialFactor;
        y *= radialFactor;

        ++offset;
    }

    for (size_t i = 0, offset = 0; i < triangleSize; ++i) {
        indices[offset + 0] = 0;
        indices[offset + 1] = i + 1;
        indices[offset + 2] = i + 2;
        offset += 3;
    }

    Render(vertices, indices, renderer2DData.whiteTexture);
}

void Renderer2D::RenderCircle(const Vector2& position, float length, const Vector4& color)
{
    RenderPolygon(position, length * 0.025, length, 0.0f, color);
}