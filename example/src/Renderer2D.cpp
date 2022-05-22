#include "Renderer2D.h"
#include <Anggur/Math/Matrix3.h>
#include <Anggur/System/Memory.h>

struct Renderer2DData
{
	Shared<Shader> shader;
	Shared<VertexArray> vertexArray;
	Shared<VertexBuffer> vertexBuffer;
	Shared<IndexBuffer> indexBuffer;

	Array<Renderer2D::Vertex> vertices;
	Array<uint32_t> indices;
	Array<Shared<Texture2D>> textures;

	size_t vertexOffset = 0;
	size_t indexOffset = 0;
	size_t textureOffset = 0;

	size_t renderCount = 0;
	size_t batchVertex = 128;
	size_t batchIndexMultiplier = 2;

	Matrix3 viewProjection;
};

static Renderer2DData data;

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
    data.vertices.assign(data.batchVertex, Vertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
    data.indices.assign(data.batchVertex * data.batchIndexMultiplier, 0);

    data.vertexBuffer = std::make_shared<VertexBuffer>();
    data.vertexBuffer->SetCapacity(sizeof(Vertex) * data.vertices.size());

    data.vertexArray = std::make_shared<VertexArray>();
    data.vertexArray->SetAttribute(0, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    data.vertexArray->SetAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
    data.vertexArray->SetAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
    data.vertexArray->SetAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texSlot));
    
    data.indexBuffer = std::make_shared<IndexBuffer>();
    data.indexBuffer->SetCapacity(sizeof(uint32_t) * data.indices.size());
}

void Renderer2D::InitializeTexturePool() 
{
    data.textures.assign(Texture2D::GetMaxSlot(), nullptr);
}

void Renderer2D::InitializeShader() 
{
    data.shader = std::make_shared<Shader>();
    data.shader->SetVertexSource(R"(
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

    data.shader->SetFragmentSource(R"(
        #version 330 core
        
        in vec4 vColor;
        in vec2 vTexCoord;
        in float vTexSlot;

        out vec4 fColor;

        uniform sampler2D uTextures[)" + std::to_string(Texture2D::GetMaxSlot()) + R"(];
        
        void main() {
            fColor = texture(uTextures[int(vTexSlot)], vTexCoord) * vColor;
        }
    )");

    data.shader->Compile();
}

void Renderer2D::SetClearColor(const Vector4& color) 
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer2D::SetBatchChunk(size_t vertex, size_t indexMultiplier) 
{
    data.batchVertex = vertex;
    data.batchIndexMultiplier = indexMultiplier;
}

void Renderer2D::Clear() 
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::SetViewProjection(const Matrix3& newViewProjection) 
{
    data.viewProjection = newViewProjection;
}

void Renderer2D::Begin() 
{
    data.renderCount = 0;
}

void Renderer2D::End() 
{
    Renderer2D::Flush();
}

void Renderer2D::Flush() 
{
    if (data.vertexOffset == 0) 
    {
        return;
    }

    data.shader->Bind();
    data.shader->SetUniformMatrix3("uViewProjection", data.viewProjection);

    data.vertexArray->Bind();
    
    data.vertexBuffer->Bind();
    data.vertexBuffer->SetData(sizeof(Vertex) * data.vertices.size(), data.vertices.data());

    data.indexBuffer->Bind();
    data.indexBuffer->SetData(sizeof(uint32_t) * data.indices.size(), data.indices.data());

    for (size_t i = 0; i < data.textureOffset; ++i) 
    {
        data.textures[i]->Bind();
    }

    glDrawElements(GL_TRIANGLES, data.indexOffset, GL_UNSIGNED_INT, nullptr);

    data.vertexOffset = 0;
    data.indexOffset = 0;
	data.textureOffset = 0;

    data.textures.clear();

    ++data.renderCount;
}

bool Renderer2D::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) 
{
    return 
        data.vertexOffset + newVertexSize > data.vertices.size() ||
        data.indexOffset + newIndexSize > data.indices.size() ||
        data.textureOffset + newTextureSize > data.textures.size();
}

void Renderer2D::Render(const Array<Vertex>& newVertices, const Array<uint32_t>& newIndices, const Shared<Texture2D>& texture) 
{
    if (IsCapacityMaxout(newVertices.size(), newIndices.size(), 1)) 
    {
        Flush();
    }

    for (size_t i = 0; i < newVertices.size(); ++i) 
    {
        auto& vertex = data.vertices[i + data.vertexOffset];

        vertex = newVertices[i];
        vertex.texSlot = data.textureOffset;
    }

    for (size_t i = 0; i < newIndices.size(); ++i) 
    {
        data.indices[i + data.indexOffset] = newIndices[i] + data.vertexOffset;
    }

    // TODO: naive method add cheking same texture ID or something
    data.textures[data.textureOffset] = texture;

    data.vertexOffset += newVertices.size();
    data.indexOffset += newIndices.size();
    data.textureOffset += 1;
}

void Renderer2D::RenderRectangle(const Vector2& position, const Vector2& size, const Shared<Texture2D>& texture, const Vector2& texturePosition, const Vector2& textureSize, const Vector4& color) 
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
