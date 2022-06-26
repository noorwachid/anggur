#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Math/Matrix3.h>
#include "Renderer2D.h"
#include <vector>

struct Renderer2DData {
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

static Renderer2DData data;

Renderer2D::Renderer2D() {
}

void Renderer2D::Initialize() {
    InitializeVertexPool();
    InitializeTexturePool();
    InitializeShader();
}

void Renderer2D::InitializeVertexPool() {
    data.vertices.assign(data.batchVertex, Vertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
    data.indices.assign(data.batchVertex * data.batchIndexMultiplier, 0);

    data.vertexBuffer = std::make_shared<VertexBuffer>();
    data.vertexBuffer->SetCapacity(sizeof(Vertex) * data.vertices.size());

    data.vertexArray = std::make_shared<VertexArray>();
    data.vertexArray->SetAttribute(0, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    data.vertexArray->SetAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
    data.vertexArray->SetAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureCoord));
    data.vertexArray->SetAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureSlot));
    
    data.indexBuffer = std::make_shared<IndexBuffer>();
    data.indexBuffer->SetCapacity(sizeof(uint32_t) * data.indices.size());
}

void Renderer2D::InitializeTexturePool() {
    data.textures.assign(Texture::GetMaxSlot(), nullptr);
    data.textureSlots.reserve(Texture::GetMaxSlot());

    for (int i = 0; i < Texture::GetMaxSlot(); ++i) {
        data.textureSlots.push_back(i);
    }

    uint8_t whitePixel[] = {255, 255, 255, 255};
    data.whiteTexture = std::make_shared<Texture2D>(whitePixel, 1, 1, 4);
}

void Renderer2D::InitializeShader() {
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

        uniform sampler2D uTextures[)" + std::to_string(Texture::GetMaxSlot()) + R"(];
        
        void main() {
            fColor = texture(uTextures[int(vTexSlot)], vTexCoord) * vColor;
        }
    )");

    data.shader->Compile();
}

void Renderer2D::SetBatchChunk(size_t vertex, size_t indexMultiplier) {
    data.batchVertex = vertex;
    data.batchIndexMultiplier = indexMultiplier;
}

void Renderer2D::Clear(const Vector4& color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::SetViewport(const Vector2& size) 
{
    glViewport(0, 0, size.x, size.y);
}

void Renderer2D::SetViewport(const Vector2& position, const Vector2& size) {
    glViewport(position.x, position.y, size.x, size.y);
}

void Renderer2D::SetViewProjection(const Matrix3& newViewProjection)
{
    data.viewProjection = newViewProjection;
}

void Renderer2D::Begin() {
    data.renderCount = 0;

    FlushData();
}

void Renderer2D::Begin(const Matrix3& viewProjection) {
    SetViewProjection(viewProjection);
    Begin();
}

void Renderer2D::End() {
    Flush();
}

void Renderer2D::Flush() {
    if (data.vertexOffset == 0) {
        return;
    }

    for (size_t i = 0; i < data.textureOffset; ++i) {
        data.textures[i]->Bind(i);
    }

    data.shader->Bind();
    data.shader->SetUniformMatrix3("uViewProjection", data.viewProjection);
    data.shader->SetUniformInt("uTextures", data.textureOffset, data.textureSlots.data());

    data.vertexArray->Bind();
    
    data.vertexBuffer->Bind();
    data.vertexBuffer->SetData(sizeof(Vertex) * data.vertexOffset, data.vertices.data());

    data.indexBuffer->Bind();
    data.indexBuffer->SetData(sizeof(uint32_t) * data.indexOffset, data.indices.data());

    glDrawElements(GL_TRIANGLES, data.indexOffset, GL_UNSIGNED_INT, nullptr);

    FlushData();

    ++data.renderCount;
}

void Renderer2D::FlushData() {
    data.vertexOffset = 0;
    data.indexOffset = 0;
	data.textureOffset = 0;
}

bool Renderer2D::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) {
    return 
        data.vertexOffset + newVertexSize > data.vertices.size() ||
        data.indexOffset + newIndexSize > data.indices.size() ||
        data.textureOffset + newTextureSize > data.textures.size();
}

void Renderer2D::Render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) {
    if (IsCapacityMaxout(newVertices.size(), newIndices.size(), 1)) {
        Flush();
    }

    // Find or add new texture slot 
    int textureSlot = 0;

    // This code only create one branch
    for (; textureSlot < data.textureOffset && data.textures[textureSlot]->GetID() != texture->GetID(); ++textureSlot);

    if (textureSlot == data.textureOffset) {
        textureSlot = data.textureOffset;
        data.textures[data.textureOffset] = texture;
        data.textureOffset += 1;
    }

    for (size_t i = 0; i < newVertices.size(); ++i) {
        auto& vertex = data.vertices[i + data.vertexOffset];

        vertex = newVertices[i];
        vertex.textureSlot = textureSlot;
    }

    for (size_t i = 0; i < newIndices.size(); ++i) {
        data.indices[i + data.indexOffset] = newIndices[i] + data.vertexOffset;
    }

    data.vertexOffset += newVertices.size();
    data.indexOffset += newIndices.size();
}

void Renderer2D::RenderRectangle(const Vector2& position, const Vector2& size, const Vector4& color) {
    Render(
        {
            // position                                               // color                                     // texCoord          
            Vertex(Vector2(position.x,          position.y),          Vector4(color.x, color.y, color.z, color.w), Vector2(0.0f, 0.0f)),
            Vertex(Vector2(position.x + size.x, position.y),	      Vector4(color.x, color.y, color.z, color.w), Vector2(1.0f, 0.0f)),
            Vertex(Vector2(position.x,          position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0.0f, 1.0f)),
            Vertex(Vector2(position.x + size.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(1.0f, 1.0f)),
        }, {
            0, 1, 2,
            2, 3, 1,
        }, 
        data.whiteTexture
    );
}

void Renderer2D::RenderRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition, const Vector2& textureSize, const Vector4& color) {
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
